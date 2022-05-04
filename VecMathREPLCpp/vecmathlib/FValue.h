#pragma once
class FValue
{
public:
	FValue();
	FValue(float);
	FValue(float,float);
	FValue(float,float,float);

	FValue(const FValue& other);
	FValue& operator=(FValue other);
	FValue(FValue&& other) noexcept;
	FValue& operator=(FValue&& rhs) noexcept;

	~FValue() {
		if (m_Values != nullptr) {
			delete[] m_Values;
		}
	}

	void print();

private:
	int m_Dimension{ 0 };
	float* m_Values{ nullptr };
};
