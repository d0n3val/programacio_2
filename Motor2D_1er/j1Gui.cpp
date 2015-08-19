#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(j1IniReader* conf)
{
	LOG("Loading GUI");
	bool ret = true;

	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");


	return true;
}

// class Gui ---------------------------------------------------
void Gui::Draw() const
{
	App->render->SetViewPort(rect);
	InnerDraw();
	App->render->ResetViewPort();
}

// class GuiImage ---------------------------------------------------
GuiImage::GuiImage(SDL_Texture* texture) : Gui(), texture(texture)
{}

GuiImage::~GuiImage()
{}

void GuiImage::InnerDraw() const
{
	App->render->Blit(texture, rect.x, rect.y);
}