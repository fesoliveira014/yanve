#pragma once

#include <common.h>
#include <graphics/gl/version.h>

namespace yanve::gl
{

namespace extensions 
{

#define __extensions(index, vendor, extension, _requiredVersion, _coreVersion) \
  struct extension {                                                      \
      enum: std::size_t { Index = index };                                \
      constexpr static Version requiredVersion() { return Version::_requiredVersion; } \
      constexpr static Version coreVersion() { return Version::_coreVersion; } \
      constexpr static const char* string() { return "GL_" #vendor "_" #extension; } \
  };

namespace AMD {
    __extensions(  0,AMD,transform_feedback3_lines_triangles, GL210, None) // #397
    __extensions(  1,AMD,vertex_shader_layer,             GL210,  None) // #417
    __extensions(  2,AMD,shader_trinary_minmax,           GL210,  None) // #428
} namespace ARB {
    __extensions( 10,ARB,texture_rectangle,               GL210, GL310) // #38
    __extensions( 11,ARB,color_buffer_float,              GL210, GL300) // #39
    __extensions( 12,ARB,half_float_pixel,                GL210, GL300) // #40
    __extensions( 13,ARB,texture_float,                   GL210, GL300) // #41
    __extensions( 14,ARB,depth_buffer_float,              GL210, GL300) // #43
    __extensions( 15,ARB,draw_instanced,                  GL210, GL310) // #44
    __extensions( 16,ARB,framebuffer_object,              GL210, GL300) // #45
    __extensions( 17,ARB,framebuffer_sRGB,                GL210, GL300) // #46
    __extensions( 18,ARB,geometry_shader4,                GL210, GL320) // #47
    __extensions( 19,ARB,half_float_vertex,               GL210, GL300) // #48
    __extensions( 20,ARB,instanced_arrays,                GL210, GL330) // #49
    __extensions( 21,ARB,map_buffer_range,                GL210, GL300) // #50
    __extensions( 22,ARB,texture_buffer_object,           GL210, GL310) // #51
    __extensions( 23,ARB,texture_rg,                      GL210, GL300) // #53
    __extensions( 24,ARB,vertex_array_object,             GL210, GL300) // #54
    __extensions( 25,ARB,uniform_buffer_object,           GL210, GL310) // #57
    __extensions( 26,ARB,copy_buffer,               /*?*/ GL210, GL310) // #59
    __extensions( 27,ARB,depth_clamp,               /*?*/ GL210, GL320) // #61
    __extensions( 28,ARB,draw_elements_base_vertex, /*?*/ GL210, GL320) // #62
    __extensions( 29,ARB,fragment_coord_conventions, /*?*/ GL210, GL320) // #63
    __extensions( 30,ARB,provoking_vertex,          /*?*/ GL210, GL320) // #64
    __extensions( 31,ARB,seamless_cube_map,               GL210, GL320) // #65
    __extensions( 32,ARB,sync,                            GL310, GL320) // #66
    __extensions( 33,ARB,texture_multisample,       /*?*/ GL210, GL320) // #67
    __extensions( 34,ARB,vertex_array_bgra,               GL210, GL320) // #68
    __extensions( 35,ARB,draw_buffers_blend,              GL210, GL400) // #69
    __extensions( 36,ARB,sample_shading,                  GL210, GL400) // #70
    __extensions( 37,ARB,texture_cube_map_array,    /*?*/ GL210, GL400) // #71
    __extensions( 38,ARB,texture_gather,                  GL210, GL400) // #72
    __extensions( 39,ARB,texture_query_lod,               GL210, GL400) // #73
    __extensions( 40,ARB,texture_compression_bptc,        GL310, GL420) // #77
    __extensions( 41,ARB,blend_func_extended,             GL210, GL330) // #78
    __extensions( 42,ARB,explicit_attrib_location,        GL210, GL330) // #79
    __extensions( 43,ARB,occlusion_query2,                GL210, GL330) // #80
    __extensions( 44,ARB,sampler_objects,                 GL210, GL330) // #81
    __extensions( 45,ARB,shader_bit_encoding,       /*?*/ GL210, GL330) // #82
    __extensions( 46,ARB,texture_rgb10_a2ui,              GL210, GL330) // #83
    __extensions( 47,ARB,texture_swizzle,           /*?*/ GL210, GL330) // #84
    __extensions( 48,ARB,timer_query,               /*?*/ GL210, GL330) // #85
    __extensions( 49,ARB,vertex_type_2_10_10_10_rev,      GL210, GL330) // #86
    __extensions( 50,ARB,draw_indirect,                   GL310, GL400) // #87
    __extensions( 51,ARB,gpu_shader5,                     GL320, GL400) // #88
    __extensions( 52,ARB,gpu_shader_fp64,                 GL320, GL400) // #89
    __extensions( 53,ARB,shader_subroutine,               GL320, GL400) // #90
    __extensions( 54,ARB,tessellation_shader,             GL320, GL400) // #91
    __extensions( 55,ARB,texture_buffer_object_rgb32, /*?*/ GL210, GL400) // #92
    __extensions( 56,ARB,transform_feedback2,             GL210, GL400) // #93
    __extensions( 57,ARB,transform_feedback3,             GL210, GL400) // #94
    __extensions( 58,ARB,ES2_compatibility,         /*?*/ GL210, GL410) // #95
    __extensions( 59,ARB,get_program_binary,              GL300, GL410) // #96
    __extensions( 60,ARB,separate_shader_objects,         GL210, GL410) // #97
    __extensions( 61,ARB,shader_precision,                GL400, GL410) // #98
    __extensions( 62,ARB,vertex_attrib_64bit,             GL300, GL410) // #99
    __extensions( 63,ARB,viewport_array,                  GL210, GL410) // #100
    __extensions( 64,ARB,robustness,                      GL210,  None) // #105
    __extensions( 65,ARB,base_instance,                   GL210, GL420) // #107
    __extensions( 66,ARB,shading_language_420pack,        GL300, GL420) // #108
    __extensions( 67,ARB,transform_feedback_instanced,    GL210, GL420) // #109
    __extensions( 68,ARB,compressed_texture_pixel_storage,GL210, GL420) // #110
    __extensions( 69,ARB,conservative_depth,              GL300, GL420) // #111
    __extensions( 70,ARB,internalformat_query,            GL210, GL420) // #112
    __extensions( 71,ARB,map_buffer_alignment,            GL210, GL420) // #113
    __extensions( 72,ARB,shader_atomic_counters,          GL300, GL420) // #114
    __extensions( 73,ARB,shader_image_load_store,         GL300, GL420) // #115
    __extensions( 74,ARB,shading_language_packing,  /*?*/ GL210, GL420) // #116
    __extensions( 75,ARB,texture_storage,                 GL210, GL420) // #117
    __extensions( 76,ARB,arrays_of_arrays,                GL210, GL430) // #120
    __extensions( 77,ARB,clear_buffer_object,             GL210, GL430) // #121
    __extensions( 78,ARB,compute_shader,                  GL420, GL430) // #122
    __extensions( 79,ARB,copy_image,                      GL210, GL430) // #123
    __extensions( 80,ARB,texture_view,                    GL210, GL430) // #124
    __extensions( 81,ARB,vertex_attrib_binding,           GL210, GL430) // #125
    __extensions( 82,ARB,robustness_isolation,            GL210,  None) // #126
    __extensions( 83,ARB,robustness_application_isolation, GL210, None) // #126
    __extensions( 84,ARB,robustness_share_group_isolation, GL210, None) // #126
    __extensions( 85,ARB,ES3_compatibility,               GL330, GL430) // #127
    __extensions( 86,ARB,explicit_uniform_location,       GL210, GL430) // #128
    __extensions( 87,ARB,fragment_layer_viewport,         GL300, GL430) // #129
    __extensions( 88,ARB,framebuffer_no_attachments,      GL210, GL430) // #130
    __extensions( 89,ARB,internalformat_query2,           GL210, GL430) // #131
    __extensions( 90,ARB,invalidate_subdata,              GL210, GL430) // #132
    __extensions( 91,ARB,multi_draw_indirect,             GL310, GL430) // #133
    __extensions( 92,ARB,program_interface_query,         GL210, GL430) // #134
    __extensions( 93,ARB,robust_buffer_access_behavior,   GL210, GL430) // #135
    __extensions( 94,ARB,shader_image_size,               GL420, GL430) // #136
    __extensions( 95,ARB,shader_storage_buffer_object,    GL400, GL430) // #137
    __extensions( 96,ARB,stencil_texturing,               GL210, GL430) // #138
    __extensions( 97,ARB,texture_buffer_range,            GL210, GL430) // #139
    __extensions( 98,ARB,texture_query_levels,            GL300, GL430) // #140
    __extensions( 99,ARB,texture_storage_multisample,     GL210, GL430) // #141
    __extensions(100,ARB,buffer_storage,            /*?*/ GL430, GL440) // #144
    __extensions(101,ARB,clear_texture,                   GL210, GL440) // #145
    __extensions(102,ARB,enhanced_layouts,                GL310, GL440) // #146
    __extensions(103,ARB,multi_bind,                      GL300, GL440) // #147
    __extensions(104,ARB,query_buffer_object,             GL210, GL440) // #148
    __extensions(105,ARB,texture_mirror_clamp_to_edge,    GL210, GL440) // #149
    __extensions(106,ARB,texture_stencil8,                GL210, GL440) // #150
    __extensions(107,ARB,vertex_type_10f_11f_11f_rev,     GL300, GL440) // #151
    __extensions(108,ARB,bindless_texture,                GL400,  None) // #152
    __extensions(109,ARB,compute_variable_group_size,     GL420,  None) // #153
    __extensions(110,ARB,indirect_parameters,             GL420, GL460) // #154
    __extensions(111,ARB,seamless_cubemap_per_texture,    GL320,  None) // #155
    __extensions(112,ARB,shader_draw_parameters,          GL310, GL460) // #156
    __extensions(113,ARB,shader_group_vote,               GL420, GL460) // #157
    __extensions(114,ARB,sparse_texture,                  GL210,  None) // #158
    __extensions(115,ARB,ES3_1_compatibility,             GL440, GL450) // #159
    __extensions(116,ARB,clip_control,                    GL210, GL450) // #160
    __extensions(117,ARB,conditional_render_inverted,     GL300, GL450) // #161
    __extensions(118,ARB,cull_distance,                   GL300, GL450) // #162
    __extensions(119,ARB,derivative_control,              GL400, GL450) // #163
    __extensions(120,ARB,direct_state_access,             GL210, GL450) // #164
    __extensions(121,ARB,get_texture_sub_image,           GL210, GL450) // #165
    __extensions(122,ARB,shader_texture_image_samples,    GL430, GL450) // #166
    __extensions(123,ARB,texture_barrier,                 GL210, GL450) // #167
    __extensions(124,ARB,pipeline_statistics_query,       GL300, GL460) // #171
    __extensions(125,ARB,sparse_buffer,                   GL210,  None) // #172
    __extensions(126,ARB,transform_feedback_overflow_query, GL300, GL460) // #173
    __extensions(127,ARB,ES3_2_compatibility,             GL450,  None) // #177
    __extensions(128,ARB,shader_atomic_counter_ops,       GL300, GL460) // #182
    __extensions(129,ARB,gl_spirv,                        GL330, GL460) // #190
    __extensions(130,ARB,polygon_offset_clamp,            GL330, GL460) // #193
    __extensions(131,ARB,spirv__extensionss,                GL330, GL460) // #194
    __extensions(132,ARB,texture_filter_anisotropic,      GL210, GL460) // #195
} namespace ATI {
    __extensions(133,ATI,texture_mirror_once,             GL210,  None) // #221
} namespace EXT {
    __extensions(139,EXT,texture_filter_anisotropic,      GL210,  None) // #187
    __extensions(140,EXT,texture_compression_s3tc,        GL210,  None) // #198
    /* EXT_framebuffer_object, EXT_packed_depth_stencil, EXT_framebuffer_blit,
       EXT_framebuffer_multisample replaced with ARB_framebuffer_object */
    __extensions(141,EXT,texture_mirror_clamp,            GL210,  None) // #298
    __extensions(142,EXT,texture_compression_dxt1,        GL210,  None) // #309
    __extensions(143,EXT,gpu_shader4,                     GL210, GL300) // #326
    __extensions(144,EXT,packed_float,                    GL210, GL300) // #328
    __extensions(145,EXT,texture_array,                   GL210, GL300) // #329
    __extensions(146,EXT,texture_compression_rgtc,        GL210, GL300) // #332
    __extensions(147,EXT,texture_shared_exponent,         GL210, GL300) // #333
    __extensions(148,EXT,draw_buffers2,                   GL210, GL300) // #340
    __extensions(149,EXT,texture_integer,                 GL210, GL300) // #343
    __extensions(150,EXT,transform_feedback,              GL210, GL300) // #352
    __extensions(151,EXT,texture_snorm,                   GL300, GL310) // #365
    __extensions(152,EXT,texture_sRGB_decode,             GL210,  None) // #402
    __extensions(153,EXT,shader_integer_mix,              GL300,  None) // #437
    __extensions(154,EXT,debug_label,                     GL210,  None) // #439
    __extensions(155,EXT,debug_marker,                    GL210,  None) // #440
    __extensions(156,EXT,texture_sRGB_R8,                 GL210,  None) // #534
} namespace GREMEDY {
    __extensions(157,GREMEDY,string_marker,               GL210,  None) // #311
} namespace KHR {
    __extensions(160,KHR,texture_compression_astc_ldr,    GL210,  None) // #118
    __extensions(161,KHR,texture_compression_astc_hdr,    GL210,  None) // #118
    __extensions(162,KHR,debug,                           GL210, GL430) // #119
    __extensions(163,KHR,context_flush_control,           GL210, GL450) // #168
    __extensions(164,KHR,robust_buffer_access_behavior,   GL320,  None) // #169
    __extensions(165,KHR,robustness,                      GL320, GL450) // #170
    __extensions(166,KHR,blend_equation_advanced,         GL210,  None) // #174
    __extensions(167,KHR,blend_equation_advanced_coherent, GL210, None) // #174
    __extensions(168,KHR,no_error,                        GL210, GL460) // #175
    __extensions(169,KHR,texture_compression_astc_sliced_3d, GL210, None) // #189
} namespace NV {
    __extensions(175,NV,primitive_restart,                GL210, GL310) // #285
    __extensions(176,NV,depth_buffer_float,               GL210, GL300) // #334
    __extensions(177,NV,conditional_render,               GL210, GL300) // #346
    /* NV_draw_texture not supported */                               // #430
}

}

}