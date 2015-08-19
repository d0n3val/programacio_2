#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
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

	atlas = App->tex->Load(conf->GetString("atlas"));

	return ret;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<Gui*>* item;

	for(item = elements.start; item; item = item->next)
	{
		item->data->Draw();
	}

	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<Gui*>* item;

	for(item = elements.start; item; item = item->next)
		RELEASE(item->data);

	elements.clear();

	return true;
}

// class Gui ---------------------------------------------------
void Gui::Draw() const
{
	App->render->SetViewPort(mask);
	InnerDraw();
	App->render->ResetViewPort();
}

// class GuiImage ---------------------------------------------------
GuiImage::GuiImage(SDL_Texture* texture, SDL_Rect section) : Gui(), texture(texture), section(section)
{}

GuiImage::~GuiImage()
{}

void GuiImage::InnerDraw() const
{
	App->render->Blit(texture, pos.x, pos.y, &section);
}