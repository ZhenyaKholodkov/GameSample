#ifndef UTILS_H
#define UTILS_H


inline const char* getResourcesPath()
{
	return "/resources/";
}

inline unsigned int rgbaToInt(unsigned int aR, unsigned int aG, unsigned int aB, unsigned int aA = 255)
{
	return (aR << 16) | (aG << 8) | (aB) | (aA << 24);
}
#endif // !UTILS_H
