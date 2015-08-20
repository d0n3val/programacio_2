#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Start(j1IniReader* conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	const char* t = conf->GetString("atlas");
	atlas = App->tex->Load(conf->GetString("atlas"));

	return ret;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	p2List_item<Gui*>* item;

	for(item = elements.start; item; item = item->next)
	{
		item->data->Update();
	}

	return true;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	p2List_item<Gui*>* item;

	for(item = elements.start; item; item = item->next)
	{
		item->data->Draw();
		item->data->DebugDraw();
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

// Create a simple image
GuiImage* j1Gui::CreateImage(const char* filename)
{
	GuiImage* ret = NULL;
	SDL_Texture* texture = App->tex->Load(filename);

	if(texture != NULL)
	{
		ret = new GuiImage(texture);
		elements.add(ret);
	}

	return ret;
}

// Create a simple image
GuiImage* j1Gui::CreateImage(SDL_Rect section)
{
	GuiImage* ret = NULL;

	ret = new GuiImage(atlas, section);
	elements.add(ret);

	return ret;
}

// Create a simple image
GuiText* j1Gui::CreateText(const char* text)
{
	GuiText* ret = NULL;

	if(text != NULL)
	{
		ret = new GuiText(text);
		elements.add(ret);
	}

	return ret;
}

// Create a selectable / invisible area on teh screen that respons to mouse events
GuiButton* j1Gui::CreateButton(int width, int height, j1Module* listener)
{
	GuiButton* ret = NULL;

	if(listener != NULL)
	{
		ret = new GuiButton(width, height, listener);
		elements.add(ret);
	}

	return ret;
}

// Create text input gui
GuiInput* j1Gui::CreateInput(const char* default_text, int width, j1Module* listener)
{
	GuiInput* ret = NULL;

	if(listener != NULL)
	{
		ret = new GuiInput(default_text, width, listener);
		elements.add(ret);
	}

	return ret;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

// class Gui ---------------------------------------------------
Gui::Gui() : use_mask(false), pos(0, 0), parent(NULL)
{}

// --------------------------
Gui::Gui(Gui* parent) : use_mask(false), pos(0, 0), parent(parent)
{}

// --------------------------
Gui::~Gui()
{}

// --------------------------
void Gui::ClearMask()
{
	use_mask = false;
}

// --------------------------
void Gui::SetMask(SDL_Rect mask)
{
	this->mask = mask;
	use_mask = true;
}

// --------------------------
void Gui::Draw() const
{
	if(use_mask == true)
	{
		App->render->SetViewPort(mask);
		InnerDraw();
		App->render->ResetViewPort();
	}
	else
		InnerDraw();
}

// --------------------------
void Gui::SetPos(int x, int y)
{
	pos.x = x; pos.y = y;
}

// --------------------------
iPoint Gui::GetPos() const
{
	if(parent != NULL)
		return iPoint(pos.x + parent->pos.x, pos.y + parent->pos.y);
	else
		return iPoint(pos.x, pos.y);
}

// --------------------------
void Gui::SetParent(Gui* parent)
{
	this->parent = parent;
}

// class GuiImage ---------------------------------------------------
GuiImage::GuiImage(SDL_Texture* texture) : Gui(), texture(texture)
{
	section.x = section.y = 0;
	App->tex->GetSize(texture, (uint&) section.w, (uint&) section.h);
}

// --------------------------
GuiImage::GuiImage(SDL_Texture* texture, SDL_Rect section) : Gui(), texture(texture), section(section)
{}

// --------------------------
GuiImage::~GuiImage()
{}

// --------------------------
void GuiImage::InnerDraw() const
{
	iPoint p = GetPos();
	App->render->Blit(texture, p.x, p.y, &section, 0.0f);
}

// class GuiText ---------------------------------------------------
GuiText::GuiText(const char* text) : Gui()
{
	texture = App->fonts->Print(text);
}

// --------------------------
GuiText::~GuiText()
{
	if(texture != NULL && texture != App->gui->GetAtlas())
		App->tex->UnLoad(texture);
}

// --------------------------
void GuiText::SetText(const char* text)
{
	if(texture != NULL)
		SDL_DestroyTexture(texture);

	texture = App->fonts->Print(text);
}

// --------------------------
const SDL_Texture* GuiText::GetTexture() const
{
	return texture;
}

// --------------------------
void GuiText::InnerDraw() const
{
	iPoint p = GetPos();
	App->render->Blit(texture, p.x, p.y, NULL, 0.0f);
}

// class GuiButton ---------------------------------------------------
GuiButton::GuiButton(uint width, uint height, j1Module* listener) : 
	Gui(), width(width), height(height), listener(listener), mouse_inside(false)
{
}

// --------------------------
GuiButton::~GuiButton()
{
}

// --------------------------
void GuiButton::Update()
{
	iPoint p = GetPos();
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	bool inside = (mouse.x >= p.x) && (mouse.x < (p.x + width)) &&
		(mouse.y >= p.y) && (mouse.y < (p.y + height));

	if(mouse_inside != inside)
	{
		mouse_inside = inside;
		listener->OnGui(this, (inside) ? GuiEvents::mouse_enters : GuiEvents::mouse_leaves);
	}

	if(inside == true)
	{
		if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
			listener->OnGui(this, GuiEvents::mouse_lclick);

		if(App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
			listener->OnGui(this, GuiEvents::mouse_rclick);
	}
}

// --------------------------
void GuiButton::DebugDraw() const
{
	iPoint p = GetPos();
	App->render->DrawQuad({p.x, p.y, width, height}, 255, 0, 0, 100, true, false);
}

// class GuiInput ---------------------------------------------------
GuiInput::GuiInput(const char* default, uint width, j1Module* listener) :
Gui(), listener(listener), width(width), focus(false), text(default), cursor_coords(0, 0), last_cursor(0)
{
	text.SetParent(this);
	uint w;
	App->tex->GetSize(text.GetTexture(), w, height);
}

// --------------------------
GuiInput::~GuiInput()
{}

// --------------------------
void GuiInput::Update()
{
	iPoint p = GetPos();
	iPoint mouse;
	App->input->GetMousePosition(mouse.x, mouse.y);

	bool inside = (mouse.x >= p.x) && (mouse.x < (p.x + width)) &&
		(mouse.y >= p.y) && (mouse.y < (p.y + height));

	if(App->input->GetMouseButtonDown(SDL_BUTTON_LEFT))
	{
		if(inside != focus)
		{
			focus = inside;
			listener->OnGui(this, (inside) ? GuiEvents::gain_focus : GuiEvents::lost_focus);

			if(focus == true)
			{
				SDL_Rect r = {p.x, p.y, width, height};
				App->input->StartTextInput(&r);
			}
			else
				App->input->EndTextInput();
		}
	}

	if(focus == true)
	{
		int cursor, selection;
		const char* user_input = App->input->GetTextInput(cursor, selection);

		if(user_input != input || cursor != last_cursor)
		{
			if(user_input != input)
			{
				input = user_input;
				text.SetText(user_input);
				listener->OnGui(this, GuiEvents::input_changed);
			}

			last_cursor = cursor;
			if(cursor > 0)
			{
				p2String selected;
				input.sub_string(0, cursor, selected);
				App->fonts->CalcSize(selected, cursor_coords.x, cursor_coords.y);
			}
			else
			{
				cursor_coords.x = 0;
			}
		}

		if(selection != 0)
		{
			listener->OnGui(this, GuiEvents::input_submit);
		}
	}
}

// --------------------------
void GuiInput::InnerDraw() const
{
	if(input.is_empty() == false)
		text.Draw();

	if(focus == true)
	{
		App->render->DrawQuad({pos.x + (cursor_coords.x - (CURSOR_WIDTH / 2)), pos.y, CURSOR_WIDTH, cursor_coords.y}, 255, 255, 255, 255, true, false);
	}
}

// --------------------------
void GuiInput::DebugDraw() const
{
	iPoint p = GetPos();
	App->render->DrawQuad({p.x, p.y, width, height}, 0, 255, 0, 100, true, false);
}