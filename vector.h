class vector
{
private:
	float x;
	float y;
	float z;

public:
	float getX() { return this->x; };
	void setX(float x) { this->x = x; };
	float getY() { return this->y; };
	void setY(float y) { this->y = y; };
	float getZ() { return this->z; };
	void setZ(float z) { this->z = z; };

	void print() { std::cout << "X: " << x << " Y: " << y << " Z: " << z << std::endl; };
	double module() { return sqrt(x*x + y * y + z * z); };

	vector() : x(0), y(0), z(0) {};
	vector(float x,float y,float z) : x(x), y(y), z(z) {};
	vector(const vector & other) : x(other.x), y(other.y), z(other.z) {};
	//~vector();



};
