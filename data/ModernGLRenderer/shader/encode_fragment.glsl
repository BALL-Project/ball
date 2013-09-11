
/**
 * Encodes a float value in an vec4. This is used to store a float value at higher precision (32 bit
 * instead of 8 bit).
 * The input should be between [0,1). After encoding 0 and 1 can not be distinguised.
 *
 * @param input Input to encode as vec4
 * @return vec4 Encoded float.
 **/
vec4 encode (in float data)
{
  const vec4 bitSh  = vec4(255*255*255, 255*255, 255, 1);
  const vec4 bitMsk = vec4(0, 1.0/255.0, 1.0/255.0, 1.0/255.0);
  vec4 comp = data * bitSh;
  comp = fract(comp);
  comp -= comp.xxyz * bitMsk;

  return comp;
}

/**
 * Decodes a vec4 back to an float value.
 *
 * @param rgba The vec4 to decode back to a float value.
 * @return float The decoded float value.
 **/
float decode (in vec4 rgba)
{
	const vec4 bitShifts = vec4(1.0/(255.0*255.0*255.0), 1.0/(255.0*255.0), 1.0/255.0, 1);
  return dot(rgba.xyzw, bitShifts);
}
