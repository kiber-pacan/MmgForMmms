

#include <fastgltf/core.hpp>
#include <fastgltf/types.hpp>
#include <fastgltf/glm_element_traits.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/hash.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <iostream>
#include <array>



using namespace std;


struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {

        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        return attributeDescriptions;
    }
    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

class mdl {

public:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    mdl(const char* path, int type) {
        if (type == 1) {
            loadGltf(path);
        } 
        if (type == 2) {
            loadOBJ(path);
        }
    }

    mdl() {}

    void loadOBJ(const char* filePath) {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath)) {
            throw std::runtime_error(warn + err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices{};

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};

                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = { 1.0f, 1.0f, 1.0f };

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }

    void loadGltf(const char* filePath) {
        std::cout << "Loading GLTF: {}" << filePath << std::endl;

        fastgltf::Parser parser{};

        constexpr auto gltfOptions = fastgltf::Options::DontRequireValidAssetMember | fastgltf::Options::AllowDouble | fastgltf::Options::LoadGLBBuffers | fastgltf::Options::LoadExternalBuffers;
        // fastgltf::Options::LoadExternalImages;

        fastgltf::GltfDataBuffer data;
        data.loadFromFile(filePath);

        fastgltf::Asset gltf;

        std::filesystem::path path = filePath;

        auto type = fastgltf::determineGltfFileType(&data);
        if (type == fastgltf::GltfType::glTF) {
            auto load = parser.loadGltf(&data, path.parent_path(), gltfOptions);
            if (load) {
                gltf = std::move(load.get());
            }
            else {
                std::cerr << "Failed to load glTF: " << fastgltf::to_underlying(load.error()) << std::endl;
            }
        }
        else if (type == fastgltf::GltfType::GLB) {
            auto load = parser.loadGltfBinary(&data, path.parent_path(), gltfOptions);
            if (load) {
                gltf = std::move(load.get());
            }
            else {
                std::cerr << "Failed to load glTF: " << fastgltf::to_underlying(load.error()) << std::endl;
            }
        }
        else {
            std::cerr << "Failed to determine glTF container" << std::endl;
        }


        std::vector<std::shared_ptr<fastgltf::Mesh>> meshes;

        for (fastgltf::Mesh& mesh : gltf.meshes) {
            for (auto&& p : mesh.primitives) {
                size_t initial_vtx = vertices.size();

                // load indexes
                {
                    fastgltf::Accessor& indexaccessor = gltf.accessors[p.indicesAccessor.value()];
                    indices.reserve(indices.size() + indexaccessor.count);

                    fastgltf::iterateAccessor<std::uint32_t>(gltf, indexaccessor,
                        [&](std::uint32_t idx) {
                            indices.push_back(idx + initial_vtx);
                        });
                }

                // load vertex positions
                {
                    fastgltf::Accessor& posAccessor = gltf.accessors[p.findAttribute("POSITION")->second];
                    vertices.resize(vertices.size() + posAccessor.count);

                    fastgltf::iterateAccessorWithIndex<glm::vec3>(gltf, posAccessor,
                        [&](glm::vec3 v, size_t index) {
                            Vertex newvtx;
                            newvtx.pos = v;
                            newvtx.color = glm::vec4{ 1.f };
                            newvtx.texCoord[0] = 0;
                            newvtx.texCoord[1] = 0;
                            vertices[initial_vtx + index] = newvtx;
                        });
                }

                // load UVs
                auto uv = p.findAttribute("TEXCOORD_0");
                if (uv != p.attributes.end()) {

                    fastgltf::iterateAccessorWithIndex<glm::vec2>(gltf, gltf.accessors[(*uv).second],
                        [&](glm::vec2 v, size_t index) {
                            vertices[initial_vtx + index].texCoord[0] = v.x;
                            vertices[initial_vtx + index].texCoord[1] = v.y;
                        });
                }
            }

        }
    }
};


class mdlInst {

public:
    float x, y, z;
    uint32_t id;
    const mdl model;

    mdlInst(float x, float y, float z, uint32_t id, mdl model) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->id = id;
    }
};