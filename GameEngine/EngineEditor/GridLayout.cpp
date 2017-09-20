#include "GridLayout.h"

void CGridLayout::InsertRow(int index)
{
	for (int i = index; i < m_rowCount; ++i)
	{
		vector<CGridElement*>& rowData = m_elements[i];
		for (CGridElement* element : rowData)
		{
			if (element)
			{
				element->m_rowIndex++;
			}
		}
	}
	m_elements.insert(m_elements.begin() + index, vector<CGridElement*>());
	m_elements[index].resize(m_colCount);
}

void CGridLayout::InsertCol(int index)
{
	for (int i = 0; i < m_rowCount; ++i)
	{
		vector<CGridElement*>& rowData = m_elements[i];
		for (int j = index; j < m_colCount; ++j)
		{
			if (rowData[j])
				rowData[j]->m_colIndex++;
		}
		rowData.insert(rowData.begin() + index, NULL);
	}
}

void CGridLayout::SetWeights(vector<float>& weights)
{
	m_weights = weights;
	for (int row = 0; row < m_elements.size(); ++row)
	{
		for (int i = 0; i < weights.size(); ++i)
		{
			//SRect2D rect = { 0, 0, weights[] * m_maxWidth * 0.5f, 0 };
			vector<CGridElement*>& rowData = m_elements[i];
		}
	}
}

void CGridLayout::SetValue(int rowIndex, int colIndex, CGridElement* element)
{
	m_elements[rowIndex][colIndex] = element;
	element->m_colIndex = colIndex;
	element->m_rowIndex = rowIndex;
}