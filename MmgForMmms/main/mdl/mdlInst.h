

class mdlInst {

public:
    float x, y, z;
    mdl model;
    uint32_t id;

    mdlInst(float x, float y, float z, mdl model, uint32_t id) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->model = model;
        this->id = id;
    }

    mdlInst();
};