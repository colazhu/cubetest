const char* LIGHTING_VERT = ("				\n\
const int maxLightNum = 3;				\n\
uniform vec4 u_globalAmbient;				\n\
uniform vec4 u_material_emssion;			\n\
uniform vec4 u_material_ambient;			\n\
uniform vec4 u_material_diffuse;			\n\
uniform vec4 u_material_specular;			\n\
uniform float u_material_specular_exponent;	        \n\
uniform bool u_light_enable[maxLightNum];               \n\
uniform vec4 u_light_position[maxLightNum];             \n\
uniform vec4 u_light_ambient[maxLightNum];              \n\
uniform vec4 u_light_diffuse[maxLightNum];              \n\
uniform vec4 u_light_specular[maxLightNum];             \n\
uniform vec3 u_light_direction[maxLightNum];		\n\
uniform bool u_light_compute_distance_attenuation[maxLightNum]; \n\
uniform vec3 u_light_attenuation_factors[maxLightNum];  \n\
uniform float u_light_spot_exponent[maxLightNum];	\n\
uniform float u_light_spot_cutoff_angle[maxLightNum];   \n\
vec4 doLighting(in vec4 vPosition, in vec3 vNormal, in vec3 vEye, out vec4 vSpecular) {             \n\
        const float c_zero = 0.0;                                               \n\
        const float c_one = 1.0;                                                \n\
        vSpecular = vec4(c_zero, c_zero, c_zero, c_zero);                       \n\
	vec4 color = u_material_emssion + u_material_ambient*u_globalAmbient;	\n\
        for(int i=0; i< maxLightNum; ++i){                                      \n\
                if (!u_light_enable[i])						\n\
                        continue;						\n\
                vec4 computed_color = vec4(c_zero, c_zero, c_zero, c_zero);     \n\
                vec3 h_vec;                                                     \n\
                float ndotl, ndoth;                                             \n\
                float att_factor;                                               \n\
                att_factor = c_one;                                             \n\
                vec3 VPpli;                                                     \n\
                if(u_light_position[i].w > 0.1)	{                               \n\
                        float spot_factor;                                      \n\
                        vec3 att_dist;                                          \n\
                        VPpli = u_light_position[i].xyz - vPosition.xyz;        \n\
                        if(u_light_compute_distance_attenuation[i])             \n\
                        {                                                       \n\
                                att_dist.x = 1.0;				\n\
                                att_dist.z = dot(VPpli, VPpli);                 \n\
                                att_dist.y = sqrt(att_dist.z);			\n\
                                att_factor = 1.0 / dot(att_dist,u_light_attenuation_factors[i]);    \n\
                        }							\n\
                        VPpli = normalize(VPpli);				\n\
                        if(u_light_spot_cutoff_angle[i] < 180.0) {              \n\
                                spot_factor = dot(-VPpli, u_light_direction[i]);\n\
                                if(spot_factor >= cos(radians(u_light_spot_cutoff_angle[i]))){      \n\
                                        spot_factor = pow(spot_factor, u_light_spot_exponent[i]);   \n\
                                }else{                                          \n\
                                        spot_factor = 0.0;			\n\
                                }						\n\
                                                                                \n\
                                att_factor *= spot_factor;			\n\
                        } 							\n\
                }								\n\
                else {								\n\
                    VPpli = -u_light_direction[i].xyz;				\n\
                }								\n\
                if(att_factor > c_zero)	{					\n\
			computed_color += (u_light_ambient[i] * u_material_ambient);		\n\
                        ndotl = max(c_zero, dot(vNormal, VPpli));				\n\
                        computed_color += (ndotl * u_light_diffuse[i] * u_material_diffuse);    \n\
                        vec3 e_vec = normalize(vEye - vPosition.xyz);                           \n\
                        h_vec = (VPpli + e_vec);                                \n\
                        h_vec = normalize(h_vec);                               \n\
                        ndoth = dot(vNormal, h_vec);				\n\
                        if (ndoth > 0.0) {					\n\
                            vSpecular += pow(ndoth, u_material_specular_exponent)* u_material_specular * u_light_specular[i];	\n\
                        }                                                       \n\
                }                                                               \n\
                computed_color *= att_factor;                                   \n\
                color += computed_color;                                        \n\
        }                                                                       \n\
        color.a = 1.0;                                                          \n\
        return color;                                                           \n\
}");	


#if 0
const char* TESTSHADER_VERT = STRINGIFY(
struct material {			\n\
vec4 _emssion;				\n\
vec4 _ambient;				\n\
vec4 _diffuse;				\n\
vec4 _specular;				\n\
float _specular_exponent;	        \n\
};					\n\
struct light {							\n\
	bool  _enable;						\n\
	vec4 _position;						\n\
	vec4 _ambient;						\n\
	vec4 _diffuse;						\n\
	vec4 _specular;						\n\
	vec3 _direction;					\n\
	bool _compute_distance_attenuation; \n\
	vec3 _attenuation_factors;			\n\
	float _spot_exponent;				\n\
	float _spot_cutoff_angle;			\n\
};										\n\
uniform material u_material;			\n\
uniform light u_lights[3];				\n\

struct directional_light {	
	vec3 direction;			
	vec3 halfplane;			
	vec4 ambient_color;		
	vec4 diffuse_color;		
	vec4 specular_color;	
};
							
struct spot_light {			
	vec4 position;			
	vec4 ambient_color;		
	vec4 diffuse_color;		
	vec4 specular_color;	
	vec3 spot_direction;	
	vec3 attenuation_factors;
	bool compute_distance_attenuation;
	float spot_exponent;	
	float spot_cutoff_angle;
};							
							
struct material_properties {
	vec4 ambient_color;		
	vec4 diffuse_color;		
	vec4 specular_color;	
	float specular_exponent;
};							
							
uniform material_properties uc_material;
uniform directional_light uc_light_dir;	
uniform spot_light uc_light_spot;
		
vec4									
directional_light(vec3 normal)			
{										
	vec4 computed_color = vec4( 0.0f,  0.0f,  0.0f,  0.0f);	
	float ndotl;						
	float ndoth;						
	ndotl = max(0.0f, dot(normal, uc_light_dir.direction));		
	ndoth = max(0.0f, dot(normal, uc_light_dir.halfplane));		
	computed_color += (uc_light_dir.ambient_color * uc_material.ambient_color);	
	computed_color += (ndotl * uc_light_dir.diffuse_color * uc_material.diffuse_color);
	if (ndoth > 0.0f)													
	{																	
		computed_color += (pow(ndoth, uc_material.specular_exponent) * uc_material.specular_color * uc_light_dir.specular_color);
	}																		
	return computed_color;													
}																			
vec4																		
spot_light(vec3 normal, vec4 position)										
{																			
	vec4 computed_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);				
	vec3 lightdir;															
	vec3 halfplane;															
	float ndotl, ndoth;														
	float att_factor;														
	att_factor = 1.0f;														
	lightdir = uc_light_spot.position.xyz - position.xyz;					
	if(uc_light_spot.compute_distance_attenuation)							
	{																		
		vec3 att_dist;														
		att_dist.x = 1.0f;													
		att_dist.z = dot(lightdir, lightdir);								
		att_dist.y = sqrt(att_dist.z);										
		att_factor = 1.0f / dot(att_dist, uc_light_spot.attenuation_factors);
	}																		
	lightdir = normalize(lightdir);											
	if(uc_light_spot.spot_cutoff_angle < 180.0)								
	{																		
		float spot_factor = dot(-lightdir, uc_light_spot.spot_direction);	
		if(spot_factor >= cos(radians(uc_light_spot.spot_cutoff_angle))) {	
			spot_factor = pow(spot_factor, uc_light_spot.spot_exponent);	
		}																	
		else																
		{																	
			spot_factor = 0.0f;											
		}																	
		att_factor *= spot_factor;											
	}																		
	if(att_factor > 0.0f)													
	{																		
		computed_color += (uc_light_spot.ambient_color * uc_material.ambient_color);
		ndotl = max(0.0f, dot(normal, lightdir));							
		computed_color += (ndotl * uc_light_spot.diffuse_color * uc_material.diffuse_color);
		halfplane = normalize(lightdir + vec3(0.0f, 0.0f, 1.0f));		
		ndoth = dot(normal, halfplane);										
		if (ndoth > 0.0f)													
		{																	
			computed_color += (pow(ndoth, uc_material.specular_exponent)*uc_material.specular_color * uc_light_spot.specular_color);
		}																	
		computed_color *= att_factor;										
	}																		
	return computed_color;													
}	
);
#endif
