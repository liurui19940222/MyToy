#ifndef _GUI_HORIZONTAL_LAYOUT_H_
#define _GUI_HORIZONTAL_LAYOUT_H_

#include"GUIElement.h"
#include<vector>

using namespace std;

class CGUIHorizontalLayout : public CGUIElement
{
	vector<CGUIElement*> m_members;
	vector<float> m_weights;
	vector<SRect2D> m_rects;
public:
	CGUIHorizontalLayout();

	void AddMember(CGUIElement* element, int index);
	void SetWeights(vector<float> weights);
	virtual void OnLayoutChanged(const SRect2D& rect) override;
};

#endif