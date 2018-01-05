#include"GridLayout.h"
#include"GUIManager.h"
#include<SpGameEngine\Debug.h>
#include<SpGameEngine\Converter.h>

void CGridLayoutCell::set(CGridLayoutElement* element)
{
	m_element = element;
	if (!element) return;
	element->m_cell = this;
	element->OnLayoutChanged(m_rect);
}

CGridLayout::CGridLayout()
{
	InsertCol(0);
	m_weights.push_back(1);
}

CGridLayout::CGridLayout(float maxWidth, float rowHeight)
{
	m_maxWidth = maxWidth;
	m_rowHeight = rowHeight;
	InsertCol(0);
	m_weights.push_back(1);
}

void CGridLayout::InsertRow(int index)
{
	for (int i = index; i < m_rowCount; ++i)
	{
		vector<CGridLayoutCell>& rowData = m_cells[i];
		for (CGridLayoutCell& cell : rowData)
		{
			cell.m_rowIndex++;
		}
	}
	m_cells.insert(m_cells.begin() + index, vector<CGridLayoutCell>());
	m_cells[index].resize(m_colCount);
	for (CGridLayoutCell& cell : m_cells[index])
	{
		cell.m_rowIndex = index;
	}
	m_rowCount++;
}

void CGridLayout::InsertCol(int index)
{
	for (int i = 0; i < m_rowCount; ++i)
	{
		vector<CGridLayoutCell>& rowData = m_cells[i];
		for (int j = index; j < m_colCount; ++j)
		{
			rowData[j].m_colIndex++;
		}
		rowData.insert(rowData.begin() + index, CGridLayoutCell());
	}
	m_colCount++;
}

void CGridLayout::ResizeRow(int size)
{
	if (size > m_cells.size())
	{
		for (int i = m_cells.size(); i < size; ++i)
		{
			InsertRow(i);
		}
	}
	else if (size < m_cells.size())
	{

	}
}

void CGridLayout::ResizeCol(int size)
{
	if (size > m_colCount)
	{
		for (int i = m_colCount; i < size; ++i)
		{
			InsertCol(i);
		}
	}
	else if (size < m_cells.size())
	{

	}
}

void CGridLayout::DeleteRow(int index, bool releaseElement)
{
	if (index >= m_rowCount)
		return;
	for (int i = index; i < m_rowCount; ++i)
	{
		vector<CGridLayoutCell>& rowData = m_cells[i];
		for (CGridLayoutCell& cell : rowData)
		{
			cell.m_rowIndex--;
		}
	}
	if (releaseElement)
	{
		vector<CGridLayoutCell>& row = m_cells[index];
		for (CGridLayoutCell& cell : row)
		{
			if (cell.m_element)
			{
				m_gui->Destroy((CGUIElement*)cell.m_element);
			}
		}
	}
	m_cells.erase(m_cells.begin() + index);
	m_rowCount--;
}

void CGridLayout::SetRowsVisible(int startIndex, int count, bool visible)
{
	if (startIndex + count > m_rowCount - 1)
	{
		count = m_rowCount - 1 - startIndex;
	}

	for (int row = startIndex; row < startIndex + count; ++row)
	{
		vector<CGridLayoutCell>& rowData = m_cells[row];
		for (CGridLayoutCell& cell : rowData)
		{
			cell.m_visible = visible;
			if (cell.m_element) cell.m_element->OnVisibleChanged(visible);
		}
	}
}

void CGridLayout::SetWeights(vector<float>& weights)
{
	m_weights = weights;
	UpdateLayout();
}

void CGridLayout::UpdateLayout()
{
	float total_weight = 0;
	float total_height = 0;
	float half_width = 0;
	for (int row = 0; row < m_cells.size(); ++row)
	{
		vector<CGridLayoutCell>& rowData = m_cells[row];
		total_weight = 0;
		if (rowData[0].m_visible)
			total_height += m_rowHeight;
		for (int i = 0; i < m_weights.size(); ++i)
		{
			half_width = m_weights[i] * m_maxWidth * 0.5f;
			SRect2D rect = { total_weight + half_width + m_offsetX , m_maxHeight - (total_height - m_rowHeight * 0.5f + m_offsetY), half_width, m_rowHeight * 0.5f };
			total_weight += m_weights[i] * m_maxWidth;
			rowData[i].m_rect = rect;
			if (rowData[i].m_element)
				rowData[i].m_element->OnLayoutChanged(rect);
		}
	}
}

void CGridLayout::SetValue(int rowIndex, int colIndex, CGridLayoutElement* element)
{
	m_cells[rowIndex][colIndex].set(element);
}

vector<CGridLayoutCell>& CGridLayout::operator[](int index)
{
	return m_cells[index];
}

void CGridLayout::PrintLayout()
{
	for (int row = 0; row < m_rowCount; ++row)
	{
		string str;
		for (int col = 0; col < m_colCount; ++col)
		{
			CGridLayoutCell& cell = m_cells[row][col];
			SRect2D& rect = m_cells[row][col].m_rect;
			int hasElement = m_cells[row][col].m_element ? 1 : 0;
			//str += "\tx:" + CConverter::ToString(rect.center_x) + " y:" + CConverter::ToString(rect.center_y) + " hw:" + CConverter::ToString(rect.half_size_x) + " hh:" + CConverter::ToString(rect.half_size_y) + " e:" + CConverter::ToString(hasElement);
			str += "row:" + CConverter::ToString(cell.m_rowIndex) + "\tvisible:" + CConverter::ToString(cell.m_visible) + "\te:" + CConverter::ToString(hasElement);
		}
		CDebug::Log(str);
	}
}

void CGridLayout::DrawLayout(CRawRenderer& renderer)
{
	for (int row = 0; row < m_rowCount; ++row)
	{
		for (int col = 0; col < m_colCount; ++col)
		{
			SRect2D& rect = m_cells[row][col].m_rect;
			renderer.DrawRect(rect, Color::black);
			renderer.DrawRect(SRect2D{ rect.center_x, rect.center_y, rect.half_size_x - 1, rect.half_size_y - 1 }, Color(0.2f, 0.2f, 0.2f, 1.0f));
		}
		CGridLayoutCell& cell = m_cells[row][0];
		if(cell.m_visible)
		renderer.DrawString(CConverter::FormatWString(L"row:%d ", cell.m_rowIndex), cell.m_rect, Vector3(cell.m_rect.center_x, cell.m_rect.center_y, 0.0f), Color::green, 13, EAlignment::RIGHT_MIDDLE);
	}
}

int CGridLayout::GetUnfilledRowIndex()
{
	bool hasSame = false;

	for (vector<CGridLayoutCell>& row : m_cells)
	{
		hasSame = false;
		for (CGridLayoutCell& cell : row)
		{
			if (cell.m_element)
			{
				hasSame = true;
				continue;
			}
		}
		if (!hasSame)
			return row[0].m_rowIndex;
	}
	return m_cells.size();
}