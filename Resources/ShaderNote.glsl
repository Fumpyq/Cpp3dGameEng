






// 0 - APos;
// 1 - Normal;
// 2 - Uv;
// 3-6 Transform;



//out vec4 FragColor;
//
//    in vec3 FragPos;
//    in vec2 TexCoords;
//
//uniform mat4 projectionMatrix;
//uniform mat4 viewMatrix;
//uniform vec3 lightPos;
//uniform vec3 viewPos;
//
//float ShadowCalculation(float depth)
//{{
//    float z = depth * 2.0 - 1.0;
//
//    vec4 clipSpacePosition = vec4(TexCoord * 2.0 - 1.0, z, 1.0);
//    vec4 viewSpacePosition = projMatrixInv * clipSpacePosition;
//
//    // Perspective division
//    viewSpacePosition /= viewSpacePosition.w;
//
//    vec4 worldSpacePosition = viewMatrixInv * viewSpacePosition;
//
//    worldSpacePosition.xyz;
//vec4 clipSpacePos = projectionMatrix * (viewMatrix * vec4(worldSpacePosition.xyz, 1.0));
//
//
//    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
//    // transform to [0,1] range
//    projCoords = projCoords * 0.5 + 0.5;
//    return projCoords.z > texture(shadowMap, projCoords.xy).r  ? 1.0 : 0.0;
//}}
//
//void main()
//{{                              
//    FragColor = vec4(1.0,ShadowCalculation(FragPosLightSpace),1.0, 1.0);
//}}


out vec4 FragColor;

    in vec3 FragPos;
    in vec2 UV;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(float depth)
{
    vec3 worldPos=WorldSpaceFromDepth(depth);
    vec2 LightUV  = screenspace(projectionMatrix,viewMatrix,worldPos);
    vec3 worldPos2=WorldSpaceFromDepth(texture(shadowMap, LightUV).r);

    return distS(worldPos,worldPos2) >1  ? 1.0 : 0.0;
}
float distS( vec3 A, vec3 B )
{

    vec3 C = A - B;
    return dot( C, C );

}
vec3 WorldSpaceFromDepth(float depth)
{    
    depth = depth * 2.0 - 1.0;
    vec4 v1= vec3(UV,depth,1);
	v1 = inverse(viewMatrix*projectionMatrix)*v1;
    return v1.xyz /v1.w;
}
vec2 screenspace(mat4 projectionmatrix, mat4 modelviewmatrix, vec3 position){

  vec4 temp = projectionmatrix * modelviewmatrix * vec4(position, 1.0);
  temp.xyz /= temp.w;
  return (0.5)+(temp.xy)*0.5;
}

void main()
{                             
    FragColor = vec4(1.0,ShadowCalculation(gl_FragCoord.z / gl_FragCoord.w),1.0, 1.0);
}