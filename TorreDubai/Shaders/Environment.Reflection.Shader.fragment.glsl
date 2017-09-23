#version 400

in vec3 pos_eye;
in vec3 n_eye;
uniform samplerCube gCubemapTexture;
uniform mat4 viewMatrix; // view matrix
out vec4 FragColor;

void main () {
  /* reflect ray around normal from eye to surface */
  vec3 incident_eye = normalize (pos_eye);
  vec3 normal = normalize (n_eye);

  vec3 reflected = reflect (incident_eye, normal);
  // convert from eye to world space
  reflected = vec3 (inverse (viewMatrix) * vec4 (reflected, 0.0));

  FragColor = texture (gCubemapTexture, reflected);
}