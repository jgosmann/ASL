/**
 * ShaderName: Map Coordinates
 * ShaderDescription: Functions for mapping coordinates between 0 and 1 (like
 *     fragment coordinats to a coordinate system.
 */

uniform int texWidth;
uniform int texHeight;

/**
 * Name: Zoom Factor
 * Default: 1
 * Range: 0.0001, max
 */
uniform float zoomFactor;

/**
 * Name: Translation
 * Default: vec2(0, 0)
 */
uniform vec2 translation;

vec2 mapCoords(vec2 coords) {
    vec2 texSize = vec2(texWidth, texHeight);
    float minSideLength = min(texSize.x, texSize.y);
    vec2 priorTranslation = vec2(-1, 1) * (texSize - minSideLength) / 2.0;
    return (2.0 * (coords + priorTranslation) / minSideLength - 1.0
        + vec2(-1, 1) * translation) / zoomFactor;
}

