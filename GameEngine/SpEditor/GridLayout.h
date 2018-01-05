#ifndef _GRID_LAYOUT_H_
#define _GRID_LAYOUT_H_

#include<vector>
#include<SpGameEngine\Property.h>
#include<SpGameEngine\EngineDefine.h>
#include<SpGameEngine\RawRenderer.h>
using namespace std;

class CGUIManager;
class CGridLayoutCell;

class CGridLayoutElement
{
	friend class CGridLayout;
	friend class CGridLayoutCell;
protected:
	virtual void OnLayoutChanged(const SRect2D& rect) = 0;
	virtual void OnVisibleChanged(bool visible) = 0;
public:
	CGridLayoutCell* m_cell;
};

class CGridLayoutCell
{
	friend class CGridLayout;
public:
	int m_rowIndex = 0;
	int m_colIndex = 0;
	int m_rowStride = 1;
	int m_colStride = 1;
	bool m_visible = true;
	SRect2D m_rect;
	CGridLayoutElement* m_element = NULL;

	void set(CGridLayoutElement* element);
};

class CGridLayout
{
	friend class CGUIManager;
private:
	float m_offsetX;
	float m_offsetY;
	int m_rowCount;
	int m_colCount;
	float m_maxWidth;
	float m_maxHeight;
	float m_rowHeight;
	vector<float> m_weights;
	vector<vector<CGridLayoutCell>> m_cells;
	CGUIManager* m_gui;

public:
	CGridLayout();
	CGridLayout(float maxWidth, float rowHeight);
	void InsertRow(int index);
	void InsertCol(int index);
	void ResizeRow(int size);
	void ResizeCol(int size);
	void DeleteRow(int index, bool releaseElement);
	void SetRowsVisible(int startIndex, int count, bool visible);
	void SetWeights(vector<float>& weights);
	void SetValue(int rowIndex, int colIndex, CGridLayoutElement* element);
	void UpdateLayout();
	void PrintLayout();
	void DrawLayout(CRawRenderer& renderer);
	int GetUnfilledRowIndex();
	vector<CGridLayoutCell>& operator[](int index);

	property_r<int> RowCount = _prop_r(int,
	{
		return m_rowCount;
	});

	property_r<int> ColCount = _prop_r(int,
	{
		return m_colCount;
	});

	property<int> MaxWidth = _prop(int, { 
		m_maxWidth = value; 
		UpdateLayout();
	}, { return m_maxWidth; });

	property<int> MaxHeight = _prop(int, {
		m_maxHeight = value;
		UpdateLayout();
	}, { return m_maxHeight; });

	property<int> RowHeight = _prop(int, { 
		m_rowHeight = value; 
		UpdateLayout();
	}, { return m_rowHeight; });

	property<int> OffsetX = _prop(int, {
		m_offsetX = value;
		UpdateLayout();
	}, { return m_offsetX; });

	property<int> OffsetY = _prop(int, {
		m_offsetY = value;
		UpdateLayout();
	}, { return m_offsetY; });
};

#endif