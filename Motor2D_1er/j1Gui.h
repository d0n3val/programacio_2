#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"

#define CURSOR_WIDTH 2

enum GuiEvents
{
	mouse_enters,
	mouse_leaves,
	mouse_lclick,
	mouse_rclick,
	gain_focus,
	lost_focus,
	input_changed,
	input_submit
};

// ---------------------------------------------------
class Gui
{
public:
	Gui();
	Gui(Gui* parent);
	virtual ~Gui();

	void Draw() const;
	virtual void DebugDraw() const
	{}
	virtual void Update()
	{}
	void SetMask(SDL_Rect mask);
	void ClearMask();
	void SetPos(int x, int y);
	iPoint GetPos() const;
	void SetParent(Gui* parent);

protected:

	virtual void InnerDraw() const
	{}

public:
	iPoint pos;

protected:
	Gui* parent;
	bool use_mask;
	SDL_Rect mask;
};

// ---------------------------------------------------
class GuiImage : public Gui
{
public:
	GuiImage(SDL_Texture* texture);
	GuiImage(SDL_Texture* texture, SDL_Rect section);
	virtual ~GuiImage();

protected:

	void InnerDraw() const;

private:

	SDL_Rect section;
	SDL_Texture* texture;
};

// ---------------------------------------------------
class GuiText : public Gui
{
public:
	GuiText(const char* text);
	virtual ~GuiText();

	void SetText(const char* text);
	const SDL_Texture* GetTexture() const;

protected:

	void InnerDraw() const;

private:

	SDL_Texture* texture;
};

// ---------------------------------------------------
class GuiButton : public Gui
{
public:
	GuiButton(uint width, uint height, j1Module* listener);
	virtual ~GuiButton();

	void DebugDraw() const;
	void Update();

private:

	uint width, height;
	j1Module* listener;
	bool mouse_inside;
};

// ---------------------------------------------------
class GuiInput : public Gui
{
public:
	GuiInput(const char* default_text, uint width, j1Module* listener);
	virtual ~GuiInput();

	void DebugDraw() const;
	void Update();

protected:

	void InnerDraw() const;

private:

	uint width, height;
	j1Module* listener;
	bool focus;
	GuiText text;
	p2String input;
	iPoint cursor_coords;
	int last_cursor;
};

// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before the first frame
	bool Start(j1IniReader* conf);

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Gui creation functions
	GuiImage* CreateImage(const char* filename);
	GuiImage* CreateImage(SDL_Rect atlas_section);
	GuiText* CreateText(const char* text);
	GuiButton* CreateButton(int width, int height, j1Module* listener);
	GuiInput* CreateInput(const char* default_text, int width, j1Module* listener);

	const SDL_Texture* GetAtlas() const;

private:

	p2List<Gui*> elements;
	SDL_Texture* atlas;
};

#endif // __j1GUI_H__