#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

// ---------------------------------------------------
class Gui
{
public:
	virtual ~Gui()
	{}

	void Draw() const;

protected:

	virtual void InnerDraw() const
	{}

public:
	SDL_Rect rect;
};

// ---------------------------------------------------
class GuiImage : public Gui
{
	GuiImage(SDL_Texture* texture);
	virtual ~GuiImage();

protected:

	void InnerDraw() const;

private:

	SDL_Texture* texture;
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called before render is available
	bool Awake(j1IniReader* conf);

	// Called before quitting
	bool CleanUp();

private:

	p2List<Gui*> elements;
};

#endif // __j1GUI_H__