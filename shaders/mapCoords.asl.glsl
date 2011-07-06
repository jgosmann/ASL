/*
 * ShaderName: Map Coordinates
 * ShaderDescription: Functions for mapping coordinates between 0 and 1 (like
 *     fragment coordinats to a coordinate system.
 */

uniform int texWidth;
uniform int texHeight;

/*
 * Name: Zoom Factor
 * Default: 1
 * Range: 0.0001, max
 */
/* uniform */ const float zoomFactor = 0.5;

/*
 * Name: Translation
 * Default: vec2(0, 0)
 */
/* uniform */ const vec2 translation = vec2(0, 0);

vec2 mapCoords(vec2 coords) {
    return (2.0 * coords / vec2(texWidth, texHeight) - 1.0
             + vec2(-1, 1) * translation)
         / zoomFactor;
}

