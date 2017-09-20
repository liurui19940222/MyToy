#ifndef _GRID_LAYOUT_H_
#define _GRID_LAYOUT_H_

#include<vector>
#include<GameEngine\Property.h>
#include<GameEngine\EngineDefine.h>
using namespace std;

class CGridElement
{
	friend class CGridLayout;

	int m_rowIndex;
	int m_colIndex;
	int m_rowStride;
	int m_colStride;

protected:
	virtual void OnLayoutChanged(const SRect2D& rect) = 0;
};

class CGridLayout
{
private:
	float m_maxWidth;
	int m_rowCount;
	int m_colCount;
	int m_rowHeight;
	vector<float> m_weights;
	vector<vector<CGridElement*>> m_elements;

public:
	void InsertRow(int index);
	void InsertCol(int index);
	void SetWeights(vector<float>& weights);
	void SetValue(int rowIndex, int colIndex, CGridElement* element);
	
	property_r<int> RowCount = _prop_r(int, 
	{
		return m_rowCount;
	});

	property_r<int> ColCount = _prop_r(int,
	{
		return m_colCount;
	});

	property<int> RowHeight = _prop(int, { m_rowHeight = value; }, { return m_rowHeight; });
};

#endif