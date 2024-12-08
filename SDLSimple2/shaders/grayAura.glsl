uniform sampler2D diffuse;
varying vec2 texCoordVar;

void main()
{
    vec4 colour = texture2D(diffuse, texCoordVar);
    float c = (colour.r + colour.g + colour.b) / 3.0;
    gl_FragColor = vec4(c, c, c, colour.a);
    gl_FragColor = vec4(colour.r, 0, 0, colour.a);
}
