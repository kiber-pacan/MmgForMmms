#version 450
#extension GL_ARB_shader_clock : enable                 // Using clockARB
#extension GL_EXT_shader_image_load_formatted : enable  // The folowing extension allow to pass images as function parameters

#extension GL_NV_shader_sm_builtins : require     // Debug - gl_WarpIDNV, gl_SMIDNV
#extension GL_ARB_gpu_shader_int64 : enable       // Debug - heatmap value
#extension GL_EXT_shader_realtime_clock : enable  // Debug - heatmap timing

#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require
#extension GL_EXT_buffer_reference2 : require
#extension GL_EXT_debug_printf : enable


layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

float r = 0;

void main() 
{
	uint64_t time = clockARB(); 
    gl_Position = vec4(inPosition, 0, 1);
    fragColor = vec3(0,1,r);
	r = 1;
}