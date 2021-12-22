
float min(float a, float b)
{
	return (a <= b) ? a : b;
}

float max(float a, float b)
{
	return (a >= b) ? a : b;
}

int sign(float x)
{
	if (x > 0)
		return 1;
	if (x < 0)
		return -1;
	return 0;
}