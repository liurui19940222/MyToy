#ifndef _SELF_CONTAINED_GRID_H_
#define _SELF_CONTAINED_GRID_H_

#include<GameEngine\EngineDefine.h>

/*
列的顺序必须是按下标从左至右
行的顺序必须是按下标从上至下
*/

#define MAX_ROW 10
#define MAX_COL 10

class CGridElement
{
protected:
	int m_col = 0;
	int m_row = 0;

public:
	void SetRow(int row) { m_row = row; }
	void SetCol(int col) { m_col = col; }
	int GetRow() { return m_row; }
	int GetCol() { return m_col; }

	virtual void OnFieldChanged(const FRect& rect) = 0;
};

template<typename T>
class GridField
{
	int m_col;
	int m_row;
	T m_data = T(0);
public:
	FRect m_rect;

	T get() { return m_data; }
	void set(T data) 
	{
		assert(dynamic_cast<CGridElement*>(data) != NULL);
		m_data = data; 
		m_data->SetRow(m_row);
		m_data->SetCol(m_col);
	}

	void SetRow(int row) 
	{ 
		m_row = row; 
		if (m_data) m_data->SetRow(row);
	}
	void SetCol(int col) 
	{ 
		m_col = col; 
		if (m_data) m_data->SetCol(col);
	}
	int GetRow() { return m_row; }
	int GetCol() { return m_col; }

	void AdaptColumn(const FRect& rect)
	{
		m_rect.left = rect.left;
		m_rect.right = rect.right;
		if(m_data)
		dynamic_cast<CGridElement*>(m_data)->OnFieldChanged(m_rect);
	}

	void SetRect(const FRect& rect)
	{
		m_rect = rect;
		if (m_data)
		dynamic_cast<CGridElement*>(m_data)->OnFieldChanged(m_rect);
	}
};

template<typename T>
class GridColumn
{
	int m_index = 0;
	int m_count = 0;
	GridField<T>* m_rows[MAX_ROW];

	void AddRow(GridField<T>* field, int col, int row)
	{
		field->SetCol(col);
		field->SetRow(row);
		m_rows[row] = field;
	}
	
public:
	FRect m_rect;

	GridColumn()
	{
		memset(m_rows, 0, sizeof(GridField<T>*) * MAX_ROW);
	}

	~GridColumn() 
	{
		for (int i = 0; i < m_count; ++i)
		{
			delete(m_rows[i]);
		}
	}

	void InsertRow(const FRect& rect, int index)
	{
		assert(m_count < MAX_ROW);
		GridField<T>* row = new GridField<T>();
		row->m_rect = rect;
		m_rows[m_count] = row;

		if (index == -1)
		{
			AddRow(row, m_index, m_count);
			index = m_count;
		}
		else if (index < m_count - 1)
		{
			for (int i = m_count - 1; i >= index; --i)
			{
				AddRow(m_rows[i], m_index, i + 1);
			}
			AddRow(row, m_index, index);
		}
		else
		{
			AddRow(row, m_index, index);
		}
		m_count++;
		UpdateTwoSide(index);
	}

	void UpdateAllRows()
	{
		for (int i = 0; i < m_count; ++i)
		{
			GridField<T>* field = m_rows[i];
			field->AdaptColumn(m_rect);
		}
	}

	void UpdateTwoSide(int index)
	{
		FRect rect = m_rows[index]->m_rect;
		//更新上边
		if (index > 0)
		{
			FRect rect_t = m_rows[index - 1]->m_rect;
			if (rect_t.bottom > rect.top)
				rect_t.bottom = rect.top;
			if (rect_t.top > rect.top)
				rect_t.top = rect.top;
			m_rows[index - 1]->SetRect(rect_t);
		}
		//更新下边
		if (index < m_count - 1)
		{
			FRect rect_b = m_rows[index + 1]->m_rect;
			if (rect_b.top < rect.bottom)
				rect_b.top = rect.bottom;
			if (rect_b.bottom < rect.bottom)
				rect_b.bottom = rect.bottom;
			m_rows[index + 1]->SetRect(rect_b);
		}
	}

	void UpdateRow(const FRect& rect, int rowIndex)
	{
		GridField<T>* row = m_rows[rowIndex];
		GridField<T>* neighbor = NULL;
		FRect oldRect = row->m_rect;
		float offset = 0;
		//上边 ↑
		if (row->m_rect.top > rect.top)
		{
			if (rowIndex == 0) return;
			offset = abs(row->m_rect.top - rect.top);
			neighbor = m_rows[rowIndex - 1];
			row->SetRect(FRect{ row->m_rect.left,  row->m_rect.top - offset, row->m_rect.right, row->m_rect.bottom });
			neighbor->SetRect(FRect{ neighbor->m_rect.left, neighbor->m_rect.top, neighbor->m_rect.right, neighbor->m_rect.bottom - offset });
		}
		//上边 ↓
		else if (row->m_rect.top < rect.top)
		{
			if (rowIndex == 0) return;
			offset = abs(row->m_rect.top - rect.top);
			neighbor = m_rows[rowIndex - 1];
			row->SetRect(FRect{ row->m_rect.left,  row->m_rect.top + offset, row->m_rect.right, row->m_rect.bottom });
			neighbor->SetRect(FRect{ neighbor->m_rect.left, neighbor->m_rect.top, neighbor->m_rect.right, neighbor->m_rect.bottom + offset });
		}
		//下边 ↓
		else if (row->m_rect.bottom < rect.bottom)
		{
			if (rowIndex >= m_count - 1) return;
			offset = abs(row->m_rect.bottom - rect.bottom);
			neighbor = m_rows[rowIndex + 1];
			row->SetRect(FRect{ row->m_rect.left,  row->m_rect.top, row->m_rect.right, row->m_rect.bottom + offset });
			neighbor->SetRect(FRect{ neighbor->m_rect.left, neighbor->m_rect.top + offset, neighbor->m_rect.right, neighbor->m_rect.bottom });
		}
		//下边 ↑
		else if (row->m_rect.bottom > rect.bottom)
		{
			if (rowIndex >= m_count - 1) return;
			offset = abs(row->m_rect.bottom - rect.bottom);
			neighbor = m_rows[rowIndex + 1];
			row->SetRect(FRect{ row->m_rect.left,  row->m_rect.top, row->m_rect.right, row->m_rect.bottom - offset });
			neighbor->SetRect(FRect{ neighbor->m_rect.left, neighbor->m_rect.top - offset, neighbor->m_rect.right, neighbor->m_rect.bottom });
		}
	}

	int Size() { return m_count; }
	void SetIndex(int index) 
	{ 
		m_index = index; 
		for (int i = 0; i < m_count; ++i)
		{
			GridField<T>* field = m_rows[i];
			field->SetCol(m_index);
		}
	}
	int GetIndex() { return m_index; }
	void SetRect(const FRect& rect) {
		m_rect = rect;
		UpdateAllRows();
	}

	GridField<T>& operator[](int index)
	{
		return *m_rows[index];
	}
};

template<typename T> 
class SelfContainedGrid
{
	GridColumn<T>* m_cols[MAX_COL];
	int m_count = 0;
	bool m_managedMemory;

	void Release(SelfContainedGrid<T>* grid)
	{
		for (int i = 0; i < m_count; ++i)
		{
			delete(m_cols[i]);
		}
	}

	void UpdateAllColumns()
	{
		for (int col = 0; col < m_count; ++col)
		{
			m_cols[col]->UpdateAllRows();
		}
	}

	void AddColumn(GridColumn<T>* col, int index)
	{
		col->SetIndex(index);
		m_cols[index] = col;
	}

public:
	SelfContainedGrid() : SelfContainedGrid(false) {}

	SelfContainedGrid(bool managedMemory) : m_managedMemory(managedMemory)
	{
		memset(m_cols, 0, sizeof(GridColumn<T>*) * MAX_COL);
	}

	~SelfContainedGrid()
	{
		Release(this);
	}

	void InsertColumn(const FRect& rect, int index)
	{
		assert(m_count < MAX_COL);
		assert(index <= m_count);
		GridColumn<T>* col = new GridColumn<T>();
		col->m_rect = rect;
		
		if (index == -1)
		{
			AddColumn(col, m_count);
			index = m_count;
		}
		else if (index < m_count - 1)
		{
			for (int i = m_count - 1; i >= index; --i)
			{
				AddColumn(m_cols[i], i + 1);
			}
			AddColumn(col, index);
		}
		else
		{
			AddColumn(col, index);
		}
		m_count++;
		UpdateTwoSide(index);
	}

	void UpdateColumn(const FRect& rect, int col)
	{
		GridColumn<T>* column = m_cols[col];
		GridColumn<T>* neighbor = NULL;
		FRect oldRect = column->m_rect;
		float offset = 0;
		//左边 <--
		if (column->m_rect.left < rect.left)
		{
			if (col == 0) return;
			offset = abs(column->m_rect.left - rect.left);
			neighbor = m_cols[col - 1];
			column->SetRect(FRect{ column->m_rect.left - offset,  column->m_rect.top, column->m_rect.right, column->m_rect.bottom });
			neighbor->SetRect(FRect{ neighbor->m_rect.left, neighbor->m_rect.top, neighbor->m_rect.right - offset, neighbor->m_rect.bottom });
		}
		//左边 -->
		else if (column->m_rect.left > rect.left)
		{
			if (col == 0) return;
			offset = abs(column->m_rect.left - rect.left);
			neighbor = m_cols[col - 1];
			column->SetRect(FRect{ column->m_rect.left + offset,  column->m_rect.top, column->m_rect.right, column->m_rect.bottom });
			neighbor->SetRect(FRect{ neighbor->m_rect.left, neighbor->m_rect.top, neighbor->m_rect.right + offset, neighbor->m_rect.bottom });
		}
		//右边 -->
		else if (column->m_rect.right < rect.right)
		{
			if (col >= m_count - 1) return;
			offset = abs(column->m_rect.right - rect.right);
			neighbor = m_cols[col + 1];
			column->SetRect(FRect{ column->m_rect.left,  column->m_rect.top, column->m_rect.right + offset, column->m_rect.bottom });
			neighbor->SetRect(FRect{ neighbor->m_rect.left + offset, neighbor->m_rect.top, neighbor->m_rect.right, neighbor->m_rect.bottom });
		}
		//右边 <--
		else if (column->m_rect.right > rect.right)
		{
			if (col >= m_count - 1) return;
			offset = abs(column->m_rect.right - rect.right);
			neighbor = m_cols[col + 1];
			column->SetRect(FRect{ column->m_rect.left,  column->m_rect.top, column->m_rect.right - offset, column->m_rect.bottom });
			neighbor->SetRect(FRect{ neighbor->m_rect.left - offset, neighbor->m_rect.top, neighbor->m_rect.right, neighbor->m_rect.bottom });
		}
	}

	void UpdateTwoSide(int index)
	{
		FRect rect = m_cols[index]->m_rect;
		//更新左边
		if (index > 0)
		{
			FRect rect_l = m_cols[index - 1]->m_rect;
			if (rect_l.right > rect.left)
				rect_l.right = rect.left;
			if (rect_l.left > rect.left)
				rect_l.left = rect.left;
			m_cols[index - 1]->SetRect(rect_l);
		}
		//更新右边
		if (index < m_count - 1)
		{
			FRect rect_r = m_cols[index + 1]->m_rect;
			if (rect_r.left < rect.right)
				rect_r.left = rect.right;
			if (rect_r.right < rect.right)
				rect_r.right = rect.right;
			m_cols[index + 1]->SetRect(rect_r);
		}
	}

	GridColumn<T>& operator[](int index)
	{
		return *m_cols[index];
	}

	int Size() { return m_count; }
};

#endif