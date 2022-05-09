#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;

int standar_screen_color_red = 20;
int standar_screen_color_green = 35;
int standar_screen_color_blue = 38;

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 700

#define CREAPER_WIDTH 100
#define CREAPER_HEIGHT 120

#define LEVEL1_WIDTH 2000
#define LEVEL1_HEIGHT 1200

#define TOTAL_TILE 1500
#define TILE_SIZE 40


const int CREAPER_BLOOD_DISPLAY = 80;
const int BOSS_BLOOD_DISPLAY = 240;

const int BOSS_WIDTH = 240;
const int BOSS_HEIGHT = 240;

const int BOSS_TOUCH_DAMAGE = 10;
const int BOSS_BULLET_DAMAGE = 10;
const int SHURIKEN_DAMAGE = 15;
const double LAVA_DAMAGE = 5;

const int FLOOR_PATH = 0;
const int FLOOR_WITH_SHADOW_PATH = 1;
const int WALL_SIDE_PATH = 2;
const int WALL_TOP_PATH = 3;
const int GATE_WAY_PATH = 4;
const int GATE_WAY_WITH_SHADOW_PATH = 5;
const int CORNER_PATH = 6;
const int BLACK_PATH = 7;

const int TOTAL_TILE_TYPE = 8;
SDL_Rect tile_sprite[TOTAL_TILE_TYPE];
const int total_gate_tile_level1 = 10;
const int gate[total_gate_tile_level1] = {660,689,710,739,760,789,810,839,860,889};
SDL_Rect play_area = {440,240,1120,800};

const int total_shuriken_clip = 8;
SDL_Rect shuriken_clip[total_shuriken_clip];
const int SHURIKEN_SIZE = 25;


const int Creaper_total_idle_frame = 18;
const int Creaper_total_idle_blinking_frame = 18;
const int Creaper_total_runing_frame = 12;
const int Creaper_total_run_throwing_frame = 12;
const int Creaper_total_throwing_frame = 12;
const int Creaper_total_dying_frame = 15;


SDL_Rect Creaper_Idle_frame[Creaper_total_idle_frame];
SDL_Rect Creaper_Idle_blinking_frame[Creaper_total_idle_blinking_frame];
SDL_Rect Creaper_Running_frame[Creaper_total_runing_frame];
SDL_Rect Creaper_Run_throwing_frame[Creaper_total_run_throwing_frame];
SDL_Rect Creaper_Throwing_frame[Creaper_total_throwing_frame];
SDL_Rect Creaper_Dying_frame[Creaper_total_dying_frame];


const int BOSS_total_idle_frame = 4;
const int BOSS_total_run_frame = 8;
SDL_Rect BOSS_idle_frame[BOSS_total_idle_frame];
SDL_Rect BOSS_run_frame[BOSS_total_run_frame];


const int METEO_total_frame = 18;
SDL_Rect Meteo_frame[METEO_total_frame];
const int METEO_WIDTH = 120;
const int METEO_HEIGHT = 250;
const int LAVA_SIZE = 240;
const int LAVA_RADIUS = 120;



struct input
{
	bool stand;
	bool up_;
	bool down_;
	bool left_;
	bool right_;
	bool fire_shuriken_;
};

enum FIRE_DIRECTION
{
	FIRE_UP = 0,
	FIRE_DOWN = 1,
	FIRE_LEFT = 2,
	FIRE_RIGHT = 3
};

class TEXTURE
{
public:
	TEXTURE();
	~TEXTURE();
	void set_width(int value);
	void set_height(int value);
	virtual bool load_texture(string path, SDL_Renderer* screen, bool use_set_color_key = false, Uint8 RED = 0, Uint8 GREEN = 0, Uint8 BLUE = 0);
	void render_texture(int x, int y, SDL_Rect* clip, SDL_Renderer* render, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void render_texture_type_1(int x, int y, SDL_Rect* clip, SDL_Renderer* screen);
	int get_width() const { return mWidth; }
	int get_height() const { return mHeight; }

	void setBlend_mode(SDL_BlendMode blend) { SDL_SetTextureBlendMode(mTexture, blend); }
	void setAlpha(Uint8 alpha) { SDL_SetTextureAlphaMod(mTexture, alpha); }
	void free();
private:
	SDL_Texture* mTexture;
	int mWidth, mHeight;
};


class TILE
{
public:
	TILE(const int& x, const int& y, const int& tileType);
	~TILE();
	void set_tile_type(const int& val) { mType = val; }
	void render_tile(SDL_Rect& camera, SDL_Renderer* screen);
	SDL_Rect get_mBlock() const { return mBlock; }
	int get_tile_type() const { return mType; }
private:
	SDL_Rect mBlock;
	int mType;
};

class BUTTON :public TEXTURE
{
public:
	const int total_button_clip = 4;
	enum mouse_status
	{
		mouse_out = 0,
		mouse_over = 1,
		mouse_down = 2,
		mouse_up = 3
	};
	BUTTON();
	~BUTTON();
	void set_in_use_button(const bool& val) { in_use_button = val; }
	void set_is_button_click(const bool& val) { is_button_click = val; }
	void set_button_rect(const int& x, const int& y, const int& w, const int& h)
	{
		button_rect.x = x; button_rect.y = y; button_rect.h = h; button_rect.w = w;
	}
	void set_button_clip_out(const int& x, const int& y, const int& w, const int& h)
	{
		button_clip[mouse_out].x = x;
		button_clip[mouse_out].y = y;
		button_clip[mouse_out].w = w;
		button_clip[mouse_out].h = h;
	}
	void set_button_clip_over(const int& x, const int& y, const int& w, const int& h)
	{
		button_clip[mouse_over].x = x;
		button_clip[mouse_over].y = y;
		button_clip[mouse_over].w = w;
		button_clip[mouse_over].h = h;
	}
	void set_button_clip_down(const int& x, const int& y, const int& w, const int& h)
	{
		button_clip[mouse_down].x = x;
		button_clip[mouse_down].y = y;
		button_clip[mouse_down].w = w;
		button_clip[mouse_down].h = h;
	}
	void set_button_clip_up(const int& x, const int& y, const int& w, const int& h)
	{
		button_clip[mouse_up].x = x;
		button_clip[mouse_up].y = y;
		button_clip[mouse_up].w = w;
		button_clip[mouse_up].h = h;
	}
	bool load_button(SDL_Renderer* screen, string path, bool use_set_color_key = false, Uint8 RED = 0, Uint8 GREEN = 0, Uint8 BLUE = 0);
	void handle_button(SDL_Event& e);
	void render_button(SDL_Renderer* screen);
	bool get_is_button_click() const { return is_button_click; }
	bool get_is_button_in_use() const { return in_use_button; }
private:
	SDL_Rect button_rect;
	SDL_Rect button_clip[4];
	int status;
	bool in_use_button;
	bool is_button_click;
};

BUTTON start_play_button;
BUTTON quit_button;
BUTTON mini_menu_button;
BUTTON continue_button;
BUTTON back_to_start_button;
BUTTON yes_button;
BUTTON no_button;

class BLOOD : public TEXTURE
{
public:
	BLOOD();
	~BLOOD();
	void set_begin_blood_display_width(const int& val) { remain_blood.w = val; total_blood.w = val; }
	void set_begin_blood_display_height(const int& val) { remain_blood.h = val; total_blood.h = val; }
	void reset_blood(const int& val) { remain_blood.w = val; }
	void reduce_blood_by_damge(const int &val) { remain_blood.w -= val; }
	bool load_blood(SDL_Renderer* screen, string path);
	void render_blood_bar(int x, int y, SDL_Renderer* screen);

	int get_remain_blood() const { return remain_blood.w; }
private:
	SDL_Rect remain_blood;
	SDL_Rect total_blood;
};

class BULLET
{
public:
	const int BULLET_SPEED = 10;
	BULLET();
	~BULLET();
	void set_bullet_x(const int& val) { bullet_pos.x = val; }
	void set_bullet_y(const int& val) { bullet_pos.y = val; }
	void set_is_bullet_move(const bool& val) { is_bullet_move = val; }
	void set_direc(const int& val) { direc = val; }

	int get_bullet_x() const { return bullet_pos.x; }
	int get_bullet_y() const { return bullet_pos.y; }
	bool get_is_bullet_move() const { return is_bullet_move; }

	void handle_bullet(TILE* tiles[]);
	void render_bullet(SDL_Renderer* screen, SDL_Rect& camera);
	SDL_Rect Get_Shuriken_Rect() const { return bullet_pos; }
private:
	SDL_Rect bullet_pos;
	bool is_bullet_move;
	int direc;
	int frame;
};

class BOSS_BULLET
{
public:
	const int BOSS_BULLET_SIZE = 40;
	const int BOSS_BULLET_SPEED = 10;
	enum BOSS_BULLET_DIRECTION
	{
		TOP = 0,
		TOP_RIGHT = 1,
		RIGHT = 2,
		BOTTOM_RIGHT = 3,
		BOTTOM = 4,
		BOTTOM_LEFT = 5,
		LEFT = 6,
		TOP_LEFT = 7
	};
	BOSS_BULLET();
	~BOSS_BULLET();
	void Set_Boss_Bullet_Positon(const int& x, const int& y) { BOSS_BULLET_POS.x = x; BOSS_BULLET_POS.y = y; }
	void Set_Is_Boss_Bullet_Move(const bool& val) { Is_Boss_Bullet_Move = val; }
	void Set_Boss_Fire_Direc(const int& val) { Boss_Bullet_Direction = val; }

	int Get_Boss_Bullet_x() const { return BOSS_BULLET_POS.x; }
	int Get_Boss_Bullet_y() const { return BOSS_BULLET_POS.y; }
	bool Get_Is_Boss_Bullet_Move() const { return Is_Boss_Bullet_Move; }
	SDL_Rect Get_Boss_Bullet_Rect() const { return BOSS_BULLET_POS; }

	void handle_bullet(TILE* tiles[]);
	void render_bullet(SDL_Renderer* screen, SDL_Rect& camera);
private:
	SDL_Rect BOSS_BULLET_POS;
	bool Is_Boss_Bullet_Move;
	int Boss_Bullet_Direction;
};
class METEO
{
public:
	const int METEO_SPEED = 10;
	METEO();
	~METEO();

	void Set_Boss_Meteo_Positon(const int& x, const int& y) { METEO_POS.x = x; METEO_POS.y = y; }
	void Set_Is_Boss_Meteo_Falling(const bool& val) { Is_Boss_Meteo_Falling = val; }
	void Set_Boss_Limit(const int& val) { Limit = val; }
	void Set_Is_Create_Lava_Pool(const bool& val) { Is_Create_Lava_Pool = val; }
	void Set_Lava_Position(const int& x, const int& y) { LAVA_POS.x = x; LAVA_POS.y = y; }
	void Set_red_x(const int& x, const int& y) { red_x_pos.x = x; red_x_pos.y = y; }
	void Set_Lava_Center() { lava_center_x = LAVA_POS.x + LAVA_SIZE / 2; lava_center_y = LAVA_POS.y + LAVA_SIZE / 2; }

	int Get_Boss_Bullet_x() const { return METEO_POS.x; }
	int Get_Boss_Bullet_y() const { return METEO_POS.y; }
	bool Get_Is_Boss_Bullet_Falling() const { return Is_Boss_Meteo_Falling; }
	bool Get_Is_Create_Lava_Pool()  const { return Is_Create_Lava_Pool; }
	int Get_Lava_Center_x() const { return lava_center_x; }
	int Get_Lava_Center_y() const { return lava_center_y; }

	void delay_lava_pool();
	void handle_meteo();
	void render_meteo(SDL_Renderer* screen, SDL_Rect& camera);
	void render_meteo_falling_position(SDL_Renderer* screen, SDL_Rect& camera);
	void render_lava_pool(SDL_Renderer* screen, SDL_Rect& camera);
private:
	SDL_Rect METEO_POS;
	SDL_Rect LAVA_POS;
	SDL_Rect red_x_pos;
	bool Is_Boss_Meteo_Falling;
	bool Is_Create_Lava_Pool;
	int Limit;
	int lava_center_x, lava_center_y;
	bool is_get_time;
	Uint32 rotTime;
	int frame;
};

class BOSS : public BLOOD
{
public:
	const int BOSS_BLOOD = 1000;
	const Uint32 delay_get_skills_period = 1500;
	const Uint32 delay_move_period = 2000;
	const int BOSS_MOVE_SPEED = 10;
	BOSS();
	~BOSS();
	SDL_Rect get_boss_collision_box() const { return collison_box; }
	bool Get_Is_Boss_Alive() const { return is_boss_alive; }
	void get_random_boss_skill();
	void get_random_move();
	void Set_it_time_for_boss_to_fight(const bool& val) { it_time_for_boss_to_fight = val; }

	void load_boss_bullet();
	void load_boss_meteo();
	void set_boss_start_position(const int& x, const int& y) { boss_rect.x = x; boss_rect.y = y; collison_box.x = boss_rect.x + 58; collison_box.y = boss_rect.y + 100; }
	void set_is_boss_alive(const bool& val) { is_boss_alive = val; }
	void delete_collision_box() { collison_box = {0,0,0,0}; };
	void it_time_to_defeat_player();
	void get_boss_new_position();
	void handle_boss_move();
	void move_boss_to_new_position(SDL_Renderer* screen, SDL_Rect& camera);
	void render_boss(SDL_Renderer* screen, SDL_Rect& camera, SDL_Rect bos, SDL_Rect player);
	
	void Handle_Boss_Bullet_List(SDL_Renderer* screen, SDL_Rect& camera, TILE* tiles[]);
	vector<BOSS_BULLET*> Get_Boss_Bullet_List() const { return boss_bullet_list; }
	vector<METEO*> Get_Boss_Meteo_List() const { return boss_meteo_list; }
	void set_skill_in_use(const int& val) { skills_in_use = val; }
	void clear_bullet()
	{
		if (!boss_bullet_list.empty()) boss_bullet_list.clear();
	}
	void clear_meteo()
	{
		if (!boss_meteo_list.empty()) boss_meteo_list.clear();
	}

	void generate_5_random_position();
	
	void handle_boss_life(vector<BULLET*> shuriken, int dame_deal);
	void render_boss_blood(SDL_Renderer* screen, SDL_Rect& camera);

private:
	bool it_time_for_boss_to_fight;
	SDL_Rect boss_rect;
	int new_pos_x, new_pos_y;
	bool Is_boss_move;
	int frame;
	bool is_boss_alive;
	SDL_Rect collison_box;
	SDL_RendererFlip boss_views;

	bool Is_Boss_Fire_Bullet;
	vector<BOSS_BULLET*> boss_bullet_list;
	vector<METEO*> boss_meteo_list;

	bool is_get_time;
	Uint32 Skills_rotTime;
	Uint32 Move_rotTime;

	vector<int> radom_position_x;
	vector<int> radom_position_y;

	int skills_in_use;
};

class Creaper : public BLOOD, public TEXTURE
{
public:
	const int CREAPER_HP = 100;
	const int move_distance = 5;
	enum STATUS
	{
		stand = 0,
		go_up = 1,
		go_down = 2,
		go_left = 3,
		go_right = 4
	};
	Creaper();
	~Creaper();
	SDL_Rect Get_Creaper_Collision() const { return collision_box; }
	bool Get_Is_Creaper_Alive() const { return is_creaper_alive; }
	vector<BULLET*> Get_Shuriken_List() const { return SHURIKEN; }
	

	bool Load_Reaper_Texture(SDL_Renderer* screen, string path);
	void set_is_creaper_alive(const bool& val) { is_creaper_alive = val; }
	void set_start_position(const int& x, const int& y) { creaper_rect.x = x; creaper_rect.y = y; }
	void handle_creaper(SDL_Event &e);
	void move(TILE* tiles[], SDL_Rect boss_rect);
	void set_camera(SDL_Rect& camera);
	void render_creaper(SDL_Renderer* screen, SDL_Rect &camera);
	void handle_shuriken_list(SDL_Renderer* screen, SDL_Rect& camera, TILE* tiles[]);
	void clear_shuriken()
	{
		if (!SHURIKEN.empty()) SHURIKEN.clear();
	}
	bool delay_damage_when_creaper_get_damage();
	void render_creaper_blood(SDL_Renderer* screen, SDL_Rect& camera);
	void handle_creaper_life(vector<BOSS_BULLET*> boss_bullet_list,vector<METEO*> boss_meteo_list, const int &boss_bullet_damage, const int &boss_lava_damage);
private:
	SDL_Rect creaper_rect;
	int velx, vely;
	input input_button;
	int frame;
	int status;
	SDL_RendererFlip change_direction;
	int fire_direction;

	SDL_Rect collision_box;
	vector<BULLET*> SHURIKEN;
	int current_creaper_hp;
	bool is_creaper_alive;
	Uint32 rotTime;
};

Creaper Main_CREAPER;

TEXTURE Bullet_texture;

TEXTURE map1_texture;
SDL_Window* WINDOW = NULL;
SDL_Renderer* RENDER = NULL;
TILE* map1[TOTAL_TILE];

TEXTURE boss_bullet;
TEXTURE boss;
BOSS Jungle_PIG;
TEXTURE BOSS_METEO;
TEXTURE LAVA_POOL;
TEXTURE red_x;

bool init();
bool load_media(TILE* tiles[]);
void close();

void set_creaper_frame();
void set_shuriken_frame();
bool is_player_inside_play_area(SDL_Rect player, SDL_Rect area);
void handle_gate(TILE* tiles[]);
bool check_collision(SDL_Rect A, SDL_Rect B);
bool set_tile(TILE* tiles[], string path);
bool touch_wall(SDL_Rect character, TILE* tiles[]);
void set_boss_frame();
void set_meteo_frame();
bool check_player_in_lava_pool_or_not(SDL_Rect& player, const int& lava_x, const int& lava_y);
void menu_button_handle(SDL_Event& e, bool & quit)
{
	start_play_button.handle_button(e);
	quit_button.handle_button(e);
	mini_menu_button.handle_button(e);
	continue_button.handle_button(e);
	back_to_start_button.handle_button(e);
	yes_button.handle_button(e);
	no_button.handle_button(e);
}
void render_menu(SDL_Renderer* screen, bool & quit)
{
	if (start_play_button.get_is_button_click())
	{
		start_play_button.set_in_use_button(false);
		quit_button.set_in_use_button(false);
		mini_menu_button.set_in_use_button(true);
		continue_button.set_in_use_button(false);
		back_to_start_button.set_in_use_button(false);
		yes_button.set_in_use_button(false);
		no_button.set_in_use_button(false);

		start_play_button.set_is_button_click(false);
	}
	else if (quit_button.get_is_button_click())
	{
		quit = true;
	}
	else if (mini_menu_button.get_is_button_click())
	{
		start_play_button.set_in_use_button(false);
		quit_button.set_in_use_button(false);
		mini_menu_button.set_in_use_button(false);
		continue_button.set_in_use_button(true);
		back_to_start_button.set_in_use_button(true);
		yes_button.set_in_use_button(false);
		no_button.set_in_use_button(false);

		mini_menu_button.set_is_button_click(false);
	}
	else if (continue_button.get_is_button_click())
	{
		start_play_button.set_in_use_button(false);
		quit_button.set_in_use_button(false);
		mini_menu_button.set_in_use_button(true);
		continue_button.set_in_use_button(false);
		back_to_start_button.set_in_use_button(false);
		yes_button.set_in_use_button(false);
		no_button.set_in_use_button(false);

		continue_button.set_is_button_click(false);
	}
	else if (back_to_start_button.get_is_button_click())
	{
		start_play_button.set_in_use_button(true);
		quit_button.set_in_use_button(true);
		mini_menu_button.set_in_use_button(false);
		continue_button.set_in_use_button(false);
		back_to_start_button.set_in_use_button(false);
		yes_button.set_in_use_button(false);
		no_button.set_in_use_button(false);

		back_to_start_button.set_is_button_click(false);
	}
	else if (yes_button.get_is_button_click())
	{
		start_play_button.set_in_use_button(false);
		quit_button.set_in_use_button(false);
		mini_menu_button.set_in_use_button(true);
		continue_button.set_in_use_button(false);
		back_to_start_button.set_in_use_button(false);
		yes_button.set_in_use_button(false);
		no_button.set_in_use_button(false);

		yes_button.set_is_button_click(false);
	}
	else if (no_button.get_is_button_click())
	{
		start_play_button.set_in_use_button(true);
		quit_button.set_in_use_button(true);
		mini_menu_button.set_in_use_button(false);
		continue_button.set_in_use_button(false);
		back_to_start_button.set_in_use_button(false);
		yes_button.set_in_use_button(false);
		no_button.set_in_use_button(false);

		no_button.set_is_button_click(false);
	}
	start_play_button.render_button(screen);
	quit_button.render_button(screen);
	mini_menu_button.render_button(screen);
	continue_button.render_button(screen);
	back_to_start_button.render_button(screen);
	yes_button.render_button(screen);
	no_button.render_button(screen);
}
void reset_gate(TILE * tiles[])
{
	for (int i = 0; i < total_gate_tile_level1; ++i)
	{
		if (i == 0 || i == 1) tiles[gate[i]]->set_tile_type(CORNER_PATH);
		else if (i == 2 || i == 3) tiles[gate[i]]->set_tile_type(GATE_WAY_WITH_SHADOW_PATH);
		else tiles[gate[i]]->set_tile_type(GATE_WAY_PATH);
	}
}

int main(int arc, char* argc[])
{
	if (!init())
	{
		cout << "fail at init(...)\n";
	}
	else
	{
		if (!load_media(map1))
		{
			cout << "fail at load_media(...)\n";
		}
		else
		{
			bool quit = false;
			SDL_Event e;
			SDL_Rect camera = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
			bool reset = false;
			Main_CREAPER.set_start_position(200, 550);
			Jungle_PIG.set_boss_start_position(900, 450);
			srand((unsigned int)time(NULL));

			while (!quit)
			{
				while (SDL_PollEvent(&e) !=0)
				{
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					Main_CREAPER.handle_creaper(e);
					menu_button_handle(e, quit);
				}


				if (start_play_button.get_is_button_in_use() && quit_button.get_is_button_in_use())
				{
					SDL_SetRenderDrawColor(RENDER, standar_screen_color_red, standar_screen_color_green, standar_screen_color_blue, 255);
					SDL_RenderClear(RENDER);
					render_menu(RENDER, quit);
				}
				else if (mini_menu_button.get_is_button_in_use())
				{
					if (reset)
					{
						Main_CREAPER.set_is_creaper_alive(true);
						Main_CREAPER.set_start_position(200, 550);
						Main_CREAPER.reset_blood(CREAPER_BLOOD_DISPLAY);

						Jungle_PIG.set_is_boss_alive(true);
						Jungle_PIG.set_boss_start_position(900, 450);
						Jungle_PIG.reset_blood(BOSS_BLOOD_DISPLAY);

						reset = false;
					}
					if (Main_CREAPER.get_remain_blood() <= 0 || Jungle_PIG.get_remain_blood() <= 0)
					{
						yes_button.set_in_use_button(true);
						no_button.set_in_use_button(true);
						mini_menu_button.set_in_use_button(false);
					}
					Main_CREAPER.move(map1, Jungle_PIG.get_boss_collision_box());
					Main_CREAPER.set_camera(camera);

					SDL_SetRenderDrawColor(RENDER, 0, 0, 0, 255);
					SDL_RenderClear(RENDER);

					for (int i = 0; i < TOTAL_TILE; ++i)
					{
						map1[i]->render_tile(camera, RENDER);
					}

					Jungle_PIG.it_time_to_defeat_player();
					Jungle_PIG.get_random_boss_skill();
					Jungle_PIG.get_random_move();
					Jungle_PIG.move_boss_to_new_position(RENDER, camera);
					Jungle_PIG.Handle_Boss_Bullet_List(RENDER, camera, map1);
					Jungle_PIG.render_boss(RENDER, camera, Jungle_PIG.get_boss_collision_box(), Main_CREAPER.Get_Creaper_Collision());
					Jungle_PIG.handle_boss_life(Main_CREAPER.Get_Shuriken_List(), SHURIKEN_DAMAGE);
					Jungle_PIG.render_boss_blood(RENDER, camera);

					SDL_SetRenderDrawColor(RENDER, 30, 30, 30, 255);
					

					Main_CREAPER.render_creaper(RENDER, camera);
					Main_CREAPER.handle_shuriken_list(RENDER, camera, map1);
					Main_CREAPER.handle_creaper_life(Jungle_PIG.Get_Boss_Bullet_List(), Jungle_PIG.Get_Boss_Meteo_List(), BOSS_BULLET_DAMAGE, LAVA_DAMAGE);
					Main_CREAPER.render_creaper_blood(RENDER, camera);

					handle_gate(map1);

					render_menu(RENDER, quit);
				}
				else if (continue_button.get_is_button_in_use() && back_to_start_button.get_is_button_in_use())
				{
					if (back_to_start_button.get_is_button_click())
					{
						Main_CREAPER.set_is_creaper_alive(false);
						Main_CREAPER.clear_shuriken();

						Jungle_PIG.set_is_boss_alive(false);
						Jungle_PIG.clear_bullet();
						Jungle_PIG.clear_meteo();
						Jungle_PIG.Set_it_time_for_boss_to_fight(false);
						reset_gate(map1);
						reset = true;
					}
					
					
					SDL_SetRenderDrawColor(RENDER, standar_screen_color_red, standar_screen_color_green, standar_screen_color_blue, 255);
					SDL_RenderClear(RENDER);
					render_menu(RENDER, quit);
				}
				else if (yes_button.get_is_button_in_use() && no_button.get_is_button_in_use())
				{
					if (no_button.get_is_button_click() || yes_button.get_is_button_click())
					{
						Main_CREAPER.set_is_creaper_alive(false);
						Main_CREAPER.clear_shuriken();

						Jungle_PIG.set_is_boss_alive(false);
						Jungle_PIG.clear_bullet();
						Jungle_PIG.clear_meteo();
						Jungle_PIG.Set_it_time_for_boss_to_fight(false);
						reset_gate(map1);
						reset = true;
					}
					SDL_SetRenderDrawColor(RENDER, standar_screen_color_red, standar_screen_color_green, standar_screen_color_blue, 255);
					SDL_RenderClear(RENDER);
					render_menu(RENDER, quit);
				}

				SDL_RenderPresent(RENDER);
			}
		}
	}

	return 0;
}


bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "fail to use sdl_init_video\n";
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		cout << "some error with sdl_sethint\n";
		return false;
	}

	WINDOW = SDL_CreateWindow("demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (WINDOW == NULL)
	{
		cout << "can not create window\n";
		return false;
	}

	RENDER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (RENDER == NULL)
	{
		cout << "can not create renderer\n";
		return false;
	}

	SDL_SetRenderDrawColor(RENDER, 0, 0, 0, 255);

	int png = IMG_INIT_PNG;
	if (!(IMG_Init(png) & png))
	{
		cout << "fail to use img_init_png\n";
		return false;
	}
	return true;
}
bool load_media(TILE* tiles[])
{
	bool success = true;

	if (!start_play_button.load_button(RENDER, "gametexture/game_menu_button/play_button.png"))
	{
		cout << "fail to load button texture\n";
		return false;
	}
	else
	{
		start_play_button.set_width(450);
		start_play_button.set_height(150);
		start_play_button.set_button_rect((SCREEN_WIDTH-450)/2, 350, 450, 150);
		start_play_button.set_button_clip_out(7, 30, 586, 156);
		start_play_button.set_button_clip_over(7, 223, 586, 156);
		start_play_button.set_button_clip_down(7, 422, 586, 156);
		start_play_button.set_button_clip_up(7, 223, 586, 156);
		start_play_button.set_in_use_button(true);
	}

	if (!quit_button.load_button(RENDER, "gametexture/game_menu_button/quit_button.png"))
	{
		cout << "fail to load button texture\n";
		return false;
	}
	else
	{
		quit_button.set_width(450);
		quit_button.set_height(150);
		quit_button.set_button_rect((SCREEN_WIDTH - 450) / 2, 500, 450, 150);
		quit_button.set_button_clip_out(6, 27, 588, 157);
		quit_button.set_button_clip_over(6, 222, 588, 157);
		quit_button.set_button_clip_down(6, 422, 588, 157);
		quit_button.set_button_clip_up(6, 222, 588, 157);
		quit_button.set_in_use_button(true);
	}

	if (!mini_menu_button.load_button(RENDER, "gametexture/game_menu_button/mini_menu.png", true, 20,35,38))
	{
		cout << "fail to load button texture\n";
		return false;
	}
	else
	{
		mini_menu_button.set_width(75);
		mini_menu_button.set_height(75);
		mini_menu_button.set_button_rect(SCREEN_WIDTH-100, 0, 75, 75);
		mini_menu_button.set_button_clip_out(40, 27, 120, 145);
		mini_menu_button.set_button_clip_over(40, 227, 120, 145);
		mini_menu_button.set_button_clip_down(40, 428, 120, 145);
		mini_menu_button.set_button_clip_up(40, 227, 120, 145);
		mini_menu_button.set_in_use_button(false);
	}

	if (!continue_button.load_button(RENDER, "gametexture/game_menu_button/continue_button.png"))
	{
		cout << "fail to load button texture\n";
		return false;
	}
	else
	{
		continue_button.set_width(360);
		continue_button.set_height(120);
		continue_button.set_button_rect((SCREEN_WIDTH-360)/2, (SCREEN_HEIGHT/2 - 120), 360, 120);
		continue_button.set_button_clip_out(75, 20, 448, 158);
		continue_button.set_button_clip_over(80, 195, 448, 158);
		continue_button.set_button_clip_down(75, 370, 448, 158);
		continue_button.set_button_clip_up(80, 195, 448, 158);
		continue_button.set_in_use_button(false);
	}

	if (!back_to_start_button.load_button(RENDER, "gametexture/game_menu_button/back_button.png"))
	{
		cout << "fail to load button texture\n";
		return false;
	}
	else
	{
		back_to_start_button.set_width(360);
		back_to_start_button.set_height(120);
		back_to_start_button.set_button_rect((SCREEN_WIDTH-360)/2, (SCREEN_HEIGHT/2), 360, 120);
		back_to_start_button.set_button_clip_out(79, 14, 420, 158);
		back_to_start_button.set_button_clip_over(79, 215, 420, 158);
		back_to_start_button.set_button_clip_down(79, 414, 420, 158);
		back_to_start_button.set_button_clip_up(79, 215, 420, 158);
		back_to_start_button.set_in_use_button(false);
	}

	if (!yes_button.load_button(RENDER, "gametexture/game_menu_button/yes_button.png"))
	{
		cout << "error with load menu: fail to load yes button\n";
		return false;
	}
	else
	{
		yes_button.set_width(200);
		yes_button.set_height(100);
		yes_button.set_button_rect(0, 0, 200, 100);
		yes_button.set_button_clip_out(111, 22, 369, 158);
		yes_button.set_button_clip_over(111, 222, 369, 158);
		yes_button.set_button_clip_down(111, 422, 369, 158);
		yes_button.set_button_clip_up(111, 222, 369, 158);
		yes_button.set_in_use_button(false);
	}

	if (!no_button.load_button(RENDER, "gametexture/game_menu_button/no_button.png"))
	{
		cout << "fail to load no button\n";
		return false;
	}
	else
	{
		no_button.set_width(200);
		no_button.set_height(100);
		no_button.set_button_rect(0, 100, 200, 100);
		no_button.set_button_clip_out(111, 22, 369, 158);
		no_button.set_button_clip_over(111, 222, 369, 158);
		no_button.set_button_clip_down(111, 422, 369, 158);
		no_button.set_button_clip_up(111, 222, 369, 158);
		no_button.set_in_use_button(false);
	}

	if (!Main_CREAPER.Load_Reaper_Texture(RENDER, "gametexture/reaper_man_1/reaper.png"))
	{
		cout << "error with load reaper: can not load texture\n";
		success = false;
	}


	if (!Main_CREAPER.load_blood(RENDER, "gametexture/blood.png"))
	{
		cout << "error with creaper blood: fail to load blood from file\n";
		success = false;
	}
	else
	{
		Main_CREAPER.set_begin_blood_display_width(CREAPER_BLOOD_DISPLAY);
		Main_CREAPER.set_begin_blood_display_height(5);
	}

	if (!Jungle_PIG.load_blood(RENDER, "gametexture/blood.png"))
	{
		cout << "error with boss blood: fail to load blood from file\n";
		success = false;
	}
	else
	{
		Jungle_PIG.set_begin_blood_display_width(BOSS_BLOOD_DISPLAY);
		Jungle_PIG.set_begin_blood_display_height(10);
	}

	if (success) set_creaper_frame();

	if (!Bullet_texture.load_texture("gametexture/reaper_man_1/bone_shuriken.png", RENDER))
	{
		cout << "can not load shuriken\n";
		success = false;
	}
	else
	{
		Bullet_texture.set_width(SHURIKEN_SIZE);
		Bullet_texture.set_height(SHURIKEN_SIZE);
		set_shuriken_frame();
	}
	
	if (!map1_texture.load_texture("gametexture/path.png", RENDER))
	{
		cout << "fail to load tile texture\n";
		success = false;
	}
	else
	{
		if (!set_tile(tiles, "map.txt"))
		{
			cout << "fail to load tile map\n";
			success = false;
		}

		map1_texture.set_height(TILE_SIZE);
		map1_texture.set_width(TILE_SIZE);
	}

	if (!boss.load_texture("gametexture/Untitled-1.png",RENDER,true, 12,7,207))
	{
		cout << "fail to load boss texture\n";
		success = false;
	}
	else
	{
		boss.set_height(BOSS_HEIGHT);
		boss.set_width(BOSS_WIDTH);
		set_boss_frame();
	}

	if (!boss_bullet.load_texture("gametexture/boss_bullet.png", RENDER, true,0,0,0))
	{
		cout << "error with boss template: fail to load boss bullet\n";
		success = false;
	}
	else
	{
		boss_bullet.set_width(50);
		boss_bullet.set_height(50);
	}

	if (!BOSS_METEO.load_texture("gametexture/meteo.png", RENDER))
	{
		cout << "error with boss template: fail to load meteo\n";
		success = false;
	}
	else
	{
		BOSS_METEO.set_width(METEO_WIDTH);
		BOSS_METEO.set_height(METEO_HEIGHT);
		set_meteo_frame();
	}

	if (!LAVA_POOL.load_texture("gametexture/red-circle.png", RENDER))
	{
		cout << "error with boss template: fail to load lava pool\n";
		success = false;
	}
	else
	{
		LAVA_POOL.setBlend_mode(SDL_BLENDMODE_BLEND);
		LAVA_POOL.setAlpha(150);
		LAVA_POOL.set_width(240);
		LAVA_POOL.set_height(240);
	}

	if (!red_x.load_texture("gametexture/red_x.png", RENDER))
	{
		cout << "error with load boss template: fail to load red x\n";
		success = false;
	}
	else
	{
		red_x.set_width(40);
		red_x.set_height(40);
	}

	return success;
}
void close()
{
	SDL_DestroyRenderer(RENDER);
	RENDER = NULL;
	SDL_DestroyWindow(WINDOW);
	WINDOW = NULL;

	IMG_Quit();
	SDL_Quit();
}
void set_creaper_frame()
{
	// Idle frame 
	
	Creaper_Idle_frame[0] = { 0,0,270,270 };
	Creaper_Idle_frame[1] = { 270,0,270,270 };
	Creaper_Idle_frame[2] = { 540,0,270,270 };
	Creaper_Idle_frame[3] = { 810,0,270,270 };
	Creaper_Idle_frame[4] = { 1080,0,270,270 };
	Creaper_Idle_frame[5] = { 1350,0,270,270 };
	Creaper_Idle_frame[6] = { 1620,0,270,270 };
	Creaper_Idle_frame[7] = { 1890,0,270,270 };
	Creaper_Idle_frame[8] = { 2160,0,270,270 };
	Creaper_Idle_frame[9] = { 2430,0,270,270 };
	Creaper_Idle_frame[10] = { 2700,0,270,270 };
	Creaper_Idle_frame[11] = { 2970,0,270,270 };
	Creaper_Idle_frame[12] = { 3240,0,270,270 };
	Creaper_Idle_frame[13] = { 3510,0,270,270 };
	Creaper_Idle_frame[14] = { 3780,0,270,270 };
	Creaper_Idle_frame[15] = { 4050,0,270,270 };
	Creaper_Idle_frame[16] = { 4320,0,270,270 };
	Creaper_Idle_frame[17] = { 4590,0,270,270 };

	// Idle blinking frame
	Creaper_Idle_blinking_frame[0] = { 0,0,270,270 };
	Creaper_Idle_blinking_frame[1] = { 270,0,270,270 };
	Creaper_Idle_blinking_frame[2] = { 540,0,270,270 };
	Creaper_Idle_blinking_frame[3] = { 810,0,270,270 };
	Creaper_Idle_blinking_frame[4] = { 1080,0,270,270 };
	Creaper_Idle_blinking_frame[5] = { 1350,0,270,270 };
	Creaper_Idle_blinking_frame[6] = { 1620,0,270,270 };
	Creaper_Idle_blinking_frame[7] = { 1890,0,270,270 };
	Creaper_Idle_blinking_frame[8] = { 2160,0,270,270 };
	Creaper_Idle_blinking_frame[9] = { 2430,0,270,270 };
	Creaper_Idle_blinking_frame[10] = { 2700,0,270,270 };
	Creaper_Idle_blinking_frame[11] = { 2970,0,270,270 };
	Creaper_Idle_blinking_frame[12] = { 3240,0,270,270 };
	Creaper_Idle_blinking_frame[13] = { 3510,0,270,270 };
	Creaper_Idle_blinking_frame[14] = { 3780,0,270,270 };
	Creaper_Idle_blinking_frame[15] = { 4050,0,270,270 };
	Creaper_Idle_blinking_frame[16] = { 4320,0,270,270 };
	Creaper_Idle_blinking_frame[17] = { 4590,0,270,270 };

	// Running frame
	Creaper_Running_frame[0] = { 0,270,270,270 };
	Creaper_Running_frame[1] = { 270,270,270,270 };
	Creaper_Running_frame[2] = { 540,270,270,270 };
	Creaper_Running_frame[3] = { 810,270,270,270 };
	Creaper_Running_frame[4] = { 1080,270,270,270 };
	Creaper_Running_frame[5] = { 1350,270,270,270 };
	Creaper_Running_frame[6] = { 1620,270,270,270 };
	Creaper_Running_frame[7] = { 1890,270,270,270 };
	Creaper_Running_frame[8] = { 2160,270,270,270 };
	Creaper_Running_frame[9] = { 2430,270,270,270 };
	Creaper_Running_frame[10] = { 2700,270,270,270 };
	Creaper_Running_frame[11] = { 2970,270,270,270 };

	// Run throwing frame
	Creaper_Run_throwing_frame[0] = { 0,0,270,270 };
	Creaper_Run_throwing_frame[1] = { 270,0,270,270 };
	Creaper_Run_throwing_frame[2] = { 540,0,270,270 };
	Creaper_Run_throwing_frame[3] = { 810,0,270,270 };
	Creaper_Run_throwing_frame[4] = { 1080,0,270,270 };
	Creaper_Run_throwing_frame[5] = { 1350,0,270,270 };
	Creaper_Run_throwing_frame[6] = { 1620,0,270,270 };
	Creaper_Run_throwing_frame[7] = { 1890,0,270,270 };
	Creaper_Run_throwing_frame[8] = { 2160,0,270,270 };
	Creaper_Run_throwing_frame[9] = { 2430,0,270,270 };
	Creaper_Run_throwing_frame[10] = { 2700,0,270,270 };
	Creaper_Run_throwing_frame[11] = { 2970,0,270,270 };

	// Throwing frame
	Creaper_Throwing_frame[0] = { 0,540,270,270 };
	Creaper_Throwing_frame[1] = { 270,540,270,270 };
	Creaper_Throwing_frame[2] = { 540,540,270,270 };
	Creaper_Throwing_frame[3] = { 810,540,270,270 };
	Creaper_Throwing_frame[4] = { 1080,540,270,270 };
	Creaper_Throwing_frame[5] = { 1350,540,270,270 };
	Creaper_Throwing_frame[6] = { 1620,540,270,270 };
	Creaper_Throwing_frame[7] = { 1890,540,270,270 };
	Creaper_Throwing_frame[8] = { 2160,540,270,270 };
	Creaper_Throwing_frame[9] = { 2430,540,270,270 };
	Creaper_Throwing_frame[10] = { 2700,540,270,270 };
	Creaper_Throwing_frame[11] = { 2970,540,270,270 };

	// Dying frame
	Creaper_Dying_frame[0] = { 0,0,270,270 };
	Creaper_Dying_frame[1] = { 270,0,270,270 };
	Creaper_Dying_frame[2] = { 540,0,270,270 };
	Creaper_Dying_frame[3] = { 810,0,270,270 };
	Creaper_Dying_frame[4] = { 1080,0,270,270 };
	Creaper_Dying_frame[5] = { 1350,0,270,270 };
	Creaper_Dying_frame[6] = { 1620,0,270,270 };
	Creaper_Dying_frame[7] = { 1890,0,270,270 };
	Creaper_Dying_frame[8] = { 2160,0,270,270 };
	Creaper_Dying_frame[9] = { 2430,0,270,270 };
	Creaper_Dying_frame[10] = { 2700,0,270,270 };
	Creaper_Dying_frame[11] = { 2970,0,270,270 };
	Creaper_Dying_frame[12] = { 3240,0,270,270 };
	Creaper_Dying_frame[13] = { 3510,0,270,270 };
	Creaper_Dying_frame[14] = { 3780,0,270,270 };
}
void set_shuriken_frame()
{
	for (int i = 0; i < total_shuriken_clip; ++i)
	{
		shuriken_clip[i] = { i * 200, 0,200,200 };
	}
}
bool is_player_inside_play_area(SDL_Rect player, SDL_Rect area)
{
	int player_feet = player.y + player.h, player_left = player.x, player_right = player.x + player.w;
	int area_top = area.y, area_bottom = area.y + area.h, area_left = area.x, area_right = area.x + area.w;
	
	if (player_left <= area_left) return false;
	if (player_right >= area_right) return false;
	if (player_feet <= area_top) return false;
	if (player_feet >= area_bottom) return false;

	return true;
}
void handle_gate(TILE * tiles[])
{
	if (is_player_inside_play_area(Main_CREAPER.Get_Creaper_Collision(), play_area) && Jungle_PIG.Get_Is_Boss_Alive())
	{
		for (int i = 0; i < total_gate_tile_level1; ++i)
		{
			tiles[gate[i]]->set_tile_type(WALL_TOP_PATH);
		}
	}
	else if (!Jungle_PIG.Get_Is_Boss_Alive())
	{
		for (int i = 0; i < total_gate_tile_level1; ++i)
		{
			if (i == 0 || i == 1) tiles[gate[i]]->set_tile_type(CORNER_PATH);
			else if (i == 2 || i == 3) tiles[gate[i]]->set_tile_type(GATE_WAY_WITH_SHADOW_PATH);
			else tiles[gate[i]]->set_tile_type(GATE_WAY_PATH);
		}
	}
}
bool check_collision(SDL_Rect A, SDL_Rect B)
{
	int topA = A.y, bottomA = A.y + A.h, leftA = A.x, rightA = A.x + A.w;
	int topB = B.y, bottomB = B.y + B.h, leftB = B.x, rightB = B.x + B.w;
	if (topA >= bottomB) return false;
	if (bottomA <= topB) return false;
	if (leftA >= rightB) return false;
	if (rightA <= leftB) return false;

	return true;
}
bool set_tile(TILE* tiles[], string path)
{
	fstream file;
	file.open(path);

	if (!file.is_open())
	{
		cout << "fail to open file map";
		return false;
	}
	else
	{
		int k = 0;
		for (int i = 0; i < LEVEL1_HEIGHT; i += TILE_SIZE)
		{
			for (int j = 0; j < LEVEL1_WIDTH; j += TILE_SIZE)
			{
				int tile_type = -1;
				file >> tile_type;
				if (file.fail())
				{
					cout << "error loading map: file not found\n";
					return false;
				}
				if (tile_type >= FLOOR_PATH && tile_type <= BLACK_PATH)
				{
					tiles[k] = new TILE(j, i, tile_type);
					++k;
				}
				else
				{
					cout << "error loading map: invalid tile type\n";
				}
			}
		}
	}


	tile_sprite[0] = { 0,0,32,32 };
	tile_sprite[1] = { 32,0,32,32 };
	tile_sprite[2] = { 64,0,31,31 };
	tile_sprite[3] = { 96,0,32,32 };
	tile_sprite[4] = { 128,0,32,32 };
	tile_sprite[5] = { 160,0,32,32 };
	tile_sprite[6] = { 192,0,32,32 };
	tile_sprite[7] = { 224,0,32,32 };

	return true;
}
bool touch_wall(SDL_Rect object, TILE* tiles[])
{
	for (int i = 0; i < TOTAL_TILE; ++i)
	{
		if (tiles[i]->get_tile_type() == WALL_TOP_PATH)
		{
			if (check_collision(object, tiles[i]->get_mBlock()))
			{
				return true;
			}
		}
	}
	return false;
}
void set_boss_frame()
{
	BOSS_idle_frame[0] = { 196,260,920,800 };
	BOSS_idle_frame[1] = { 1040,260,920,800 };
	BOSS_idle_frame[2] = { 1915,260,920,800 };
	BOSS_idle_frame[3] = { 2830,260,920,800 };

	BOSS_run_frame[0] = { 200,2120,920,800 };
	BOSS_run_frame[1] = { 1070,2120,920,800 };
	BOSS_run_frame[2] = { 1950,2120,920,800 };
	BOSS_run_frame[3] = { 2800,2120,920,800 };
	BOSS_run_frame[4] = { 3660,2120,920,800 };
	BOSS_run_frame[5] = { 4555,2120,920,800 };
	BOSS_run_frame[6] = { 5420,2120,920,800 };
	BOSS_run_frame[7] = { 6275,2120,920,800 };
}
void set_meteo_frame()
{
	Meteo_frame[0] = { 670,30,120,250 };
	Meteo_frame[1] = { 670,260,120,250 };
	Meteo_frame[2] = { 674,490,120,250 };
	Meteo_frame[3] = { 674,720,120,250 };
	Meteo_frame[4] = { 535,30,120,250 };
	Meteo_frame[5] = { 535,260,120,250 };
	Meteo_frame[6] = { 535,490,120,250 };
	Meteo_frame[7] = { 535,720,120,250 };
	Meteo_frame[8] = { 395,30,120,250 };
	Meteo_frame[9] = { 395,260,120,250 };
	Meteo_frame[10] = { 395,490,120,250 };
	Meteo_frame[11] = { 395,720,120,250 };
	Meteo_frame[12] = { 258,30,120,250 };
	Meteo_frame[13] = { 258,260,120,250 };
	Meteo_frame[14] = { 258,490,120,250 };
	Meteo_frame[15] = { 258,720,120,250 };
	Meteo_frame[16] = { 125,260,120,250 };
	Meteo_frame[17] = { 125,490,120,250 };
}
bool check_player_in_lava_pool_or_not(SDL_Rect& player, const int& lava_x, const int& lava_y)
{
	int player_pos_x = player.x + player.w / 2;
	int player_pos_y = player.y + player.h;

	double distance_from_player_to_lava_center = (double)sqrt((double)pow(lava_x - player_pos_x, 2) + (double)pow(lava_y - player_pos_y, 2));
	if (distance_from_player_to_lava_center <= LAVA_RADIUS) return true;
	return false;
}


TEXTURE::TEXTURE()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}
TEXTURE::~TEXTURE()
{
	free();
}
void TEXTURE::set_width(int value)
{
	mWidth = value;
}
void TEXTURE::set_height(int value)
{
	mHeight = value;
}
bool TEXTURE::load_texture(string path, SDL_Renderer* screen, bool use_set_color_key, Uint8 RED, Uint8 GREEN, Uint8 BLUE)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if (loaded_surface == NULL)
	{
		cout << "can not load surface from file\n";
		return false;
	}
	else
	{
		if (use_set_color_key)
		{
			SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, RED, GREEN, BLUE));
		}
		newTexture = SDL_CreateTextureFromSurface(screen, loaded_surface);
		if (newTexture == NULL)
		{
			cout << "can not create texture from surface\n";
			return false;
		}
		else
		{
			mWidth = loaded_surface->w;
			mHeight = loaded_surface->h;

			SDL_FreeSurface(loaded_surface);
			loaded_surface = NULL;
		}
	}
	mTexture = newTexture;
	return mTexture != NULL;
}
void TEXTURE::render_texture(int x, int y, SDL_Rect* clip, SDL_Renderer* screen, SDL_RendererFlip flip)
{
	SDL_Rect render_quad = { x,y,mWidth,mHeight };
	SDL_RenderCopyEx(screen, mTexture, clip, &render_quad, 0.0, NULL, flip);
}
void TEXTURE::render_texture_type_1(int x, int y, SDL_Rect* clip, SDL_Renderer* screen)
{
	SDL_Rect render_quad = { x,y,mWidth,mHeight };
	if (clip != NULL)
	{
		render_quad.w = clip->w;
		render_quad.h = clip->h;
	}
	SDL_RenderCopy(screen, mTexture, clip, &render_quad);
}
void TEXTURE::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}


TILE::TILE(const int& x, const int& y, const int& tileType)
{
	mBlock = { x,y,TILE_SIZE,TILE_SIZE };
	mType = tileType;
}
TILE::~TILE()
{
}
void TILE::render_tile(SDL_Rect& camera, SDL_Renderer* screen)
{
	if (check_collision(mBlock, camera))
	{
		map1_texture.render_texture(mBlock.x - camera.x, mBlock.y - camera.y, &tile_sprite[mType], screen, SDL_FLIP_NONE);
	}
}



BUTTON::BUTTON()
{
	button_rect = { 0,0,0,0 };
	for (int i = 0; i < total_button_clip; ++i) button_clip[i] = { 0,0,0,0 };
	status = mouse_out;
	in_use_button = false;
	is_button_click = false;
}
BUTTON::~BUTTON()
{
}
bool BUTTON::load_button(SDL_Renderer* screen, string path, bool use_set_color_key, Uint8 RED, Uint8 GREEN, Uint8 BLUE)
{
	bool load = TEXTURE::load_texture(path, screen, use_set_color_key, RED, GREEN, BLUE);
	return load;
}
void BUTTON::handle_button(SDL_Event& e)
{
	if ((e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) && in_use_button)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;
		if (x < button_rect.x) inside = false;
		if (x > button_rect.x + button_rect.w) inside = false;
		if (y < button_rect.y) inside = false;
		if (y > button_rect.y + button_rect.h) inside = false;

		if (inside)
		{
			switch (e.type)
			{
			case SDL_MOUSEMOTION:
				status = mouse_over;
				break;
			case SDL_MOUSEBUTTONUP:
				is_button_click = true;
				status = mouse_up;
				break;
			case SDL_MOUSEBUTTONDOWN:
				status = mouse_down;
				break;
			}
		}
		else status = mouse_out;
	}
	else return;
}
void BUTTON::render_button(SDL_Renderer* screen)
{
	if (in_use_button) TEXTURE::render_texture(button_rect.x, button_rect.y, &button_clip[status], screen);
}


BLOOD::BLOOD()
{
	remain_blood = { 0,0,0,0 };
	total_blood = { 0,20,0,0 };
}
BLOOD::~BLOOD()
{
	free();
}
bool BLOOD::load_blood(SDL_Renderer* screen, string path)
{
	bool load = load_texture(path, screen);
	if (!load)
	{
		cout << "fail to load blood texture\n";
	}
	return load;
}
void BLOOD::render_blood_bar(int x, int y, SDL_Renderer* screen)
{
	render_texture_type_1(x, y, &total_blood, screen);
	render_texture_type_1(x, y, &remain_blood, screen);
}


BULLET::BULLET()
{
	bullet_pos = { 0,0,SHURIKEN_SIZE,SHURIKEN_SIZE };
	is_bullet_move = false;
	direc = -1;
	frame = 0;
}
BULLET::~BULLET()
{
}
void BULLET::handle_bullet(TILE* tiles[])
{
	if (direc == FIRE_UP)
	{
		bullet_pos.y -= BULLET_SPEED;
		if (bullet_pos.y < 0 || check_collision(bullet_pos, Jungle_PIG.get_boss_collision_box()) || touch_wall(bullet_pos, tiles)) is_bullet_move = false;
	}

	if (direc == FIRE_DOWN)
	{
		bullet_pos.y += BULLET_SPEED;
		if (bullet_pos.y + SHURIKEN_SIZE > LEVEL1_HEIGHT || check_collision(bullet_pos, Jungle_PIG.get_boss_collision_box()) || touch_wall(bullet_pos, tiles)) is_bullet_move = false;
	}

	if (direc == FIRE_LEFT)
	{
		bullet_pos.x -= BULLET_SPEED;
		if (bullet_pos.x < 0 || check_collision(bullet_pos, Jungle_PIG.get_boss_collision_box()) || touch_wall(bullet_pos, tiles)) is_bullet_move = false;
	}

	if (direc == FIRE_RIGHT)
	{
		bullet_pos.x += BULLET_SPEED;
		if (bullet_pos.x + SHURIKEN_SIZE > LEVEL1_WIDTH || check_collision(bullet_pos, Jungle_PIG.get_boss_collision_box()) || touch_wall(bullet_pos, tiles)) is_bullet_move = false;
	}
}
void BULLET::render_bullet(SDL_Renderer* screen, SDL_Rect& camera)
{
	frame++;
	if (frame / 2 > total_shuriken_clip) frame = 0;
	Bullet_texture.render_texture(bullet_pos.x - camera.x, bullet_pos.y - camera.y, &shuriken_clip[frame / 2], screen, SDL_FLIP_NONE);
}


BOSS_BULLET::BOSS_BULLET()
{
	BOSS_BULLET_POS = { 0,0,BOSS_BULLET_SIZE,BOSS_BULLET_SIZE };
	Is_Boss_Bullet_Move = false;
	Boss_Bullet_Direction = -1;
}
BOSS_BULLET::~BOSS_BULLET()
{
}
void BOSS_BULLET::handle_bullet(TILE* tiles[])
{
	if (Boss_Bullet_Direction == TOP)
	{
		BOSS_BULLET_POS.y -= BOSS_BULLET_SPEED;
		if (BOSS_BULLET_POS.y < 0 || check_collision(BOSS_BULLET_POS, Main_CREAPER.Get_Creaper_Collision()) || touch_wall(BOSS_BULLET_POS, tiles))
		{
			Is_Boss_Bullet_Move = false;
		}
	}
	else if (Boss_Bullet_Direction == TOP_RIGHT)
	{
		BOSS_BULLET_POS.x += BOSS_BULLET_SPEED;
		BOSS_BULLET_POS.y -= BOSS_BULLET_SPEED;
		if (BOSS_BULLET_POS.y < 0 || BOSS_BULLET_POS.x + BOSS_BULLET_SIZE > LEVEL1_WIDTH || check_collision(BOSS_BULLET_POS, Main_CREAPER.Get_Creaper_Collision()) || touch_wall(BOSS_BULLET_POS, tiles))
		{
			Is_Boss_Bullet_Move = false;
		}
	}
	else if (Boss_Bullet_Direction == RIGHT)
	{
		BOSS_BULLET_POS.x += BOSS_BULLET_SPEED;
		if (BOSS_BULLET_POS.x + BOSS_BULLET_SIZE > LEVEL1_WIDTH || check_collision(BOSS_BULLET_POS, Main_CREAPER.Get_Creaper_Collision()) || touch_wall(BOSS_BULLET_POS, tiles))
		{
			Is_Boss_Bullet_Move = false;
		}
	}
	else if (Boss_Bullet_Direction == BOTTOM_RIGHT)
	{
		BOSS_BULLET_POS.x += BOSS_BULLET_SPEED;
		BOSS_BULLET_POS.y += BOSS_BULLET_SPEED;
		if (BOSS_BULLET_POS.y + BOSS_BULLET_SIZE > LEVEL1_HEIGHT || BOSS_BULLET_POS.x + BOSS_BULLET_SIZE > LEVEL1_WIDTH || check_collision(BOSS_BULLET_POS, Main_CREAPER.Get_Creaper_Collision()) || touch_wall(BOSS_BULLET_POS, tiles))
		{
			Is_Boss_Bullet_Move = false;
		}
	}
	else if (Boss_Bullet_Direction == BOTTOM)
	{
		BOSS_BULLET_POS.y += BOSS_BULLET_SPEED;
		if (BOSS_BULLET_POS.y + BOSS_BULLET_SIZE > LEVEL1_HEIGHT || check_collision(BOSS_BULLET_POS, Main_CREAPER.Get_Creaper_Collision()) || touch_wall(BOSS_BULLET_POS, tiles))
		{
			Is_Boss_Bullet_Move = false;
		}
	}
	else if (Boss_Bullet_Direction == BOTTOM_LEFT)
	{
		BOSS_BULLET_POS.x -= BOSS_BULLET_SPEED;
		BOSS_BULLET_POS.y += BOSS_BULLET_SPEED;
		if (BOSS_BULLET_POS.y + BOSS_BULLET_SIZE > LEVEL1_HEIGHT || BOSS_BULLET_POS.x < 0 || check_collision(BOSS_BULLET_POS, Main_CREAPER.Get_Creaper_Collision()) || touch_wall(BOSS_BULLET_POS, tiles))
		{
			Is_Boss_Bullet_Move = false;
		}
	}
	else if (Boss_Bullet_Direction == LEFT)
	{
		BOSS_BULLET_POS.x -= BOSS_BULLET_SPEED;
		if (BOSS_BULLET_POS.x < 0 || check_collision(BOSS_BULLET_POS, Main_CREAPER.Get_Creaper_Collision()) || touch_wall(BOSS_BULLET_POS, tiles))
		{
			Is_Boss_Bullet_Move = false;
		}
	}
	else if (Boss_Bullet_Direction == TOP_LEFT)
	{
		BOSS_BULLET_POS.x -= BOSS_BULLET_SPEED;
		BOSS_BULLET_POS.y -= BOSS_BULLET_SPEED;
		if (BOSS_BULLET_POS.y < 0 || BOSS_BULLET_POS.x < 0 || check_collision(BOSS_BULLET_POS, Main_CREAPER.Get_Creaper_Collision()) || touch_wall(BOSS_BULLET_POS, tiles))
		{
			Is_Boss_Bullet_Move = false;
		}
	}
}
void BOSS_BULLET::render_bullet(SDL_Renderer* screen, SDL_Rect& camera)
{
	boss_bullet.render_texture(BOSS_BULLET_POS.x - camera.x, BOSS_BULLET_POS.y - camera.y, NULL, screen);
}


METEO::METEO()
{
	METEO_POS = { 0,0,METEO_WIDTH, METEO_HEIGHT };
	Is_Boss_Meteo_Falling = false;
	Is_Create_Lava_Pool = false;
	Limit = 0;

	is_get_time = false;
	rotTime = 0;
	LAVA_POS = { 0,0,0,0 };
	red_x_pos = { 0,0,0,0 };

	frame = 0;

	lava_center_x = 0;
	lava_center_y = 0;
}
METEO::~METEO()
{
}
void METEO::delay_lava_pool()
{
	if (is_get_time && (SDL_GetTicks() - rotTime > 4000))
	{
		Is_Create_Lava_Pool = false;
	}
}
void METEO::handle_meteo()
{
	METEO_POS.y += METEO_SPEED;
	if (METEO_POS.y + METEO_HEIGHT > Limit)
	{
		Is_Boss_Meteo_Falling = false;
		Is_Create_Lava_Pool = true;
		is_get_time = true;
		rotTime = SDL_GetTicks();
	}
}
void METEO::render_meteo(SDL_Renderer* screen, SDL_Rect& camera)
{

	if (Is_Boss_Meteo_Falling && !Is_Create_Lava_Pool)
	{
		BOSS_METEO.render_texture(METEO_POS.x - camera.x, METEO_POS.y - camera.y, &Meteo_frame[frame], screen, SDL_FLIP_NONE);
		frame++;
		if (frame > METEO_total_frame) frame = 0;
	}
	else LAVA_POOL.render_texture(LAVA_POS.x - camera.x, LAVA_POS.y - camera.y, NULL, screen);
}
void METEO::render_lava_pool(SDL_Renderer* screen, SDL_Rect& camera)
{
	LAVA_POOL.render_texture(METEO_POS.x-camera.x, METEO_POS.y-camera.y, NULL,screen);
}
void METEO::render_meteo_falling_position(SDL_Renderer* screen, SDL_Rect& camera)
{
	red_x.render_texture(red_x_pos.x - camera.x, red_x_pos.y - camera.y, NULL, screen);
}


BOSS::BOSS()
{
	boss_rect = { 1000,1000,BOSS_WIDTH,BOSS_HEIGHT };
	new_pos_x = 0;
	new_pos_y = 0;
	Is_boss_move = 0;
	frame = 0;
	is_boss_alive = true;
	collison_box = { 0,0,130,85 };
	boss_views = SDL_FLIP_NONE;

	Is_Boss_Fire_Bullet = false;

	is_get_time = true;
	Skills_rotTime = 0;
	Move_rotTime = 0;
	radom_position_x = { 0,0,0,0,0 };
	radom_position_y = { 0,0,0,0,0 };
	skills_in_use = 0;
}
BOSS::~BOSS()
{
}
void BOSS::get_random_boss_skill()
{
	if (it_time_for_boss_to_fight && is_boss_alive)
	{
		if (is_get_time && (SDL_GetTicks() - Skills_rotTime > delay_get_skills_period))
		{
			Skills_rotTime = SDL_GetTicks();
			int random_skill = rand() % 2 + 1;
			skills_in_use = random_skill;
			if (skills_in_use == 1) load_boss_bullet();
			else if (skills_in_use == 2)
			{
				generate_5_random_position();
				load_boss_meteo();
			}
			else if (skills_in_use == 3)
			{
				get_boss_new_position();
				Is_boss_move = true;
			}
			else return;
		}
	}
	else return;
}
void BOSS::get_random_move()
{
	if (it_time_for_boss_to_fight && is_boss_alive)
	{
		if (is_get_time && (SDL_GetTicks() - Move_rotTime > delay_move_period))
		{
			Move_rotTime = SDL_GetTicks();
				get_boss_new_position();
				Is_boss_move = true;
		}
	}
	else return;
}
void BOSS::it_time_to_defeat_player()
{
	if (is_player_inside_play_area(Main_CREAPER.Get_Creaper_Collision(), play_area)) 
	{
		it_time_for_boss_to_fight = true;
	}
}
void BOSS::load_boss_bullet()
{
	for (int i = 0; i < 8; ++i)
	{
		BOSS_BULLET* newBULLET = new BOSS_BULLET;
		newBULLET->Set_Boss_Bullet_Positon(boss_rect.x + 100 , boss_rect.y + 150);
		newBULLET->Set_Boss_Fire_Direc(i);
		newBULLET->Set_Is_Boss_Bullet_Move(true);

		boss_bullet_list.push_back(newBULLET);
	}
}
void BOSS::load_boss_meteo()
{
	for (int i = 0; i < 5; ++i)
	{
		METEO* newMeteo = new METEO;
		newMeteo->Set_Boss_Meteo_Positon(radom_position_x[i], radom_position_y[i] - 1000);
		newMeteo->Set_Boss_Limit(radom_position_y[i]);
		newMeteo->Set_Is_Boss_Meteo_Falling(true);
		newMeteo->Set_Lava_Position(radom_position_x[i] - 60, radom_position_y[i] - 100);
		newMeteo->Set_Lava_Center();
		newMeteo->Set_red_x(radom_position_x[i]+40, radom_position_y[i]);
		boss_meteo_list.push_back(newMeteo);
	}
}
void BOSS::get_boss_new_position()
{
	new_pos_x = rand() % 880 + 440;
	new_pos_y = rand() % 520 + 160;
}
void BOSS::handle_boss_move()
{
	if (new_pos_x > boss_rect.x)
	{
		boss_views = SDL_FLIP_NONE;
		boss_rect.x += BOSS_MOVE_SPEED;
		collison_box.x = boss_rect.x + 58;
		if (boss_rect.x > new_pos_x)
		{
			boss_rect.x = new_pos_x;
			collison_box.x = boss_rect.x + 58;
		}
	}

	if (new_pos_x < boss_rect.x)
	{
		boss_views = SDL_FLIP_HORIZONTAL;
		boss_rect.x -= BOSS_MOVE_SPEED;
		collison_box.x = boss_rect.x + 58;
		if (boss_rect.x < new_pos_x)
		{
			boss_rect.x = new_pos_x;
			collison_box.x = boss_rect.x + 58;
		}
	}

	if (new_pos_y > boss_rect.y)
	{
		boss_rect.y += BOSS_MOVE_SPEED;
		collison_box.y = boss_rect.y + 100;
		if (boss_rect.y > new_pos_y)
		{
			boss_rect.y = new_pos_y;
			collison_box.y = boss_rect.y + 100;
		}
	}

	if (new_pos_y < boss_rect.y)
	{
		boss_rect.y -= BOSS_MOVE_SPEED;
		collison_box.y = boss_rect.y + 100;
		if (boss_rect.y < new_pos_y)
		{
			boss_rect.y = new_pos_y;
			collison_box.y = boss_rect.y + 100;
		}
	}


	if (boss_rect.x == new_pos_x && boss_rect.y == new_pos_y) Is_boss_move = false;
}
void BOSS::move_boss_to_new_position(SDL_Renderer* screen, SDL_Rect& camera)
{
	if (Is_boss_move)
	{
		red_x.render_texture(new_pos_x - camera.x + 100, new_pos_y - camera.y + 200, NULL, RENDER);
		handle_boss_move();
	}
	else return;
}
void BOSS::render_boss(SDL_Renderer* screen, SDL_Rect& camera, SDL_Rect bos, SDL_Rect player)
{
	if (check_collision(boss_rect, camera)&& is_boss_alive)
	{
		if (Is_boss_move)
		{
			boss.render_texture(boss_rect.x - camera.x, boss_rect.y - camera.y, &BOSS_run_frame[frame / 6], screen, boss_views);
			frame++;
			if (frame/6 > BOSS_total_run_frame - 1) frame = 0;
		}
		else
		{
			boss.render_texture(boss_rect.x - camera.x, boss_rect.y - camera.y, &BOSS_idle_frame[frame / 8], screen, boss_views);
			frame++;
			if (frame / 8 > BOSS_total_idle_frame - 1) frame = 0;
			
		}
	}
}
void BOSS::Handle_Boss_Bullet_List(SDL_Renderer* screen, SDL_Rect& camera,TILE* tiles[])
{
	if (boss_bullet_list.size() >= 1)
	{
		for (int i = 0; i < boss_bullet_list.size(); ++i)
		{
			if (boss_bullet_list[i] != NULL)
			{
				if (boss_bullet_list[i]->Get_Is_Boss_Bullet_Move())
				{
					boss_bullet_list[i]->handle_bullet(tiles);
					boss_bullet_list[i]->render_bullet(screen, camera);
				}
				else
				{
					boss_bullet_list.erase(boss_bullet_list.begin() + i);
				}
			}
		}
	}
	
	if (boss_meteo_list.size() >= 1)
	{
		for (int i = 0; i < boss_meteo_list.size(); ++i)
		{
			if (boss_meteo_list[i] != NULL)
			{
				if (boss_meteo_list[i]->Get_Is_Boss_Bullet_Falling() && !boss_meteo_list[i]->Get_Is_Create_Lava_Pool())
				{
					boss_meteo_list[i]->handle_meteo();
					boss_meteo_list[i]->render_meteo(screen, camera);
					boss_meteo_list[i]->render_meteo_falling_position(screen, camera);
				}
				else if (!boss_meteo_list[i]->Get_Is_Boss_Bullet_Falling() && boss_meteo_list[i]->Get_Is_Create_Lava_Pool())
				{
					boss_meteo_list[i]->delay_lava_pool();
					boss_meteo_list[i]->render_meteo(screen, camera);
				}
				else if (!boss_meteo_list[i]->Get_Is_Create_Lava_Pool())
				{
					boss_meteo_list.erase(boss_meteo_list.begin() + i);
				}
			}
		}
	}

	return;
}
void BOSS::generate_5_random_position()
{
	for (int i = 0; i < 5; ++i)
	{
		radom_position_x[i] = rand() % 880 + 441;
		radom_position_y[i] = rand() % 560 + 401;
	}
}
void BOSS::handle_boss_life(vector<BULLET*> shuriken, int damage)
{
	if (shuriken.size() >= 1)
	{
		for (int i = 0; i < shuriken.size(); ++i)
		{
			if (check_collision(shuriken[i]->Get_Shuriken_Rect(), collison_box))
			{
				shuriken[i]->set_is_bullet_move(false);
				BLOOD::reduce_blood_by_damge(damage);
				if (BLOOD::get_remain_blood() <= 0)
				{
					is_boss_alive = false;
					delete_collision_box();
				}
			}
		}
	}
}
void BOSS::render_boss_blood(SDL_Renderer* screen, SDL_Rect& camera)
{
	if (is_boss_alive) render_blood_bar(boss_rect.x - camera.x, boss_rect.y - camera.y, screen);
	else return;
}


Creaper::Creaper()
{
	creaper_rect = { 0,0,CREAPER_WIDTH, CREAPER_HEIGHT };
	velx = 0;
	vely = 0;
	input_button.stand = true;
	input_button.up_ = false;
	input_button.down_ = false;
	input_button.left_ = false;
	input_button.right_ = false;
	input_button.fire_shuriken_ = false;
	frame = 0;
	status = stand;
	change_direction = SDL_FLIP_NONE;
	fire_direction = FIRE_RIGHT;
	collision_box = { 0,0,25,55 };

	is_creaper_alive = true;
	current_creaper_hp = CREAPER_HP;
	rotTime = 0;
}
Creaper::~Creaper()
{
}
bool Creaper::Load_Reaper_Texture(SDL_Renderer* screen, string path)
{
	bool load = TEXTURE::load_texture(path, screen, true, 20, 35, 38);
	if (load)
	{
		TEXTURE::set_width(CREAPER_WIDTH);
		TEXTURE::set_height(CREAPER_HEIGHT);
	}
	return load;
}
void Creaper::handle_creaper(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP:
		{
			vely -= move_distance;
			input_button.up_ = true;
			fire_direction = FIRE_UP;
			input_button.stand = false;
			status = go_up;
			break;
		}
		case SDLK_DOWN:
		{
			vely += move_distance;
			input_button.down_ = true;
			fire_direction = FIRE_DOWN;
			input_button.stand = false;
			status = go_down;
			break;
		}
		case SDLK_LEFT:
		{
			velx -= move_distance;
			input_button.left_ = true;
			fire_direction = FIRE_LEFT;
			input_button.stand = false;
			change_direction = SDL_FLIP_HORIZONTAL;
			status = go_left;
			break;
		}
		case SDLK_RIGHT:
		{
			velx += move_distance;
			input_button.right_ = true;
			fire_direction = FIRE_RIGHT;
			input_button.stand = false;
			change_direction = SDL_FLIP_NONE;
			status = go_right;
			break;
		}
		case SDLK_SPACE:
		{
			input_button.fire_shuriken_ = true;
			BULLET* newBullet = new BULLET;
			if (fire_direction == FIRE_UP)
			{
				newBullet->set_bullet_x(creaper_rect.x + CREAPER_WIDTH / 2);
				newBullet->set_bullet_y(creaper_rect.y + 40);
			}
			else if (fire_direction == FIRE_DOWN)
			{
				newBullet->set_bullet_x(creaper_rect.x + CREAPER_WIDTH / 2);
				newBullet->set_bullet_y(creaper_rect.y + CREAPER_HEIGHT - 40);
			}
			else if (fire_direction == FIRE_LEFT)
			{
				newBullet->set_bullet_x(creaper_rect.x + 60);
				newBullet->set_bullet_y(creaper_rect.y + CREAPER_HEIGHT / 2 + 20);
			}
			else if (fire_direction == FIRE_RIGHT)
			{
				newBullet->set_bullet_x(creaper_rect.x + CREAPER_WIDTH - 70);
				newBullet->set_bullet_y(creaper_rect.y + CREAPER_HEIGHT / 2 + 20);
			}

			newBullet->set_direc(fire_direction);
			newBullet->set_is_bullet_move(true);
			SHURIKEN.push_back(newBullet);
			break;
		}
		default:
			break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: vely += move_distance; input_button.up_ = false; break;
		case SDLK_DOWN: vely -= move_distance; input_button.down_ = false; break;
		case SDLK_LEFT: velx += move_distance; input_button.left_ = false; break;
		case SDLK_RIGHT: velx -= move_distance; input_button.right_ = false; break;
		default:
			break;
		}
		if (input_button.stand == false && !(input_button.up_|| input_button.down_|| input_button.left_ || input_button.right_))
		{
			input_button.stand = true;
			input_button.fire_shuriken_ = false;
			status = stand;
		}
	}
}
void Creaper::move(TILE* tiles[], SDL_Rect boss_rect)
{
	creaper_rect.x += velx;
	collision_box.x = creaper_rect.x + 37;
	if (collision_box.x  < 0 || collision_box.x + 25 > LEVEL1_WIDTH || touch_wall(collision_box, tiles) || check_collision(boss_rect, collision_box))
	{
		if (check_collision(collision_box, boss_rect))
		{
			if (delay_damage_when_creaper_get_damage())
			{
				BLOOD::reduce_blood_by_damge(BOSS_TOUCH_DAMAGE);
				if (BLOOD::get_remain_blood() <= 0) is_creaper_alive = false;
			}
		}
		creaper_rect.x -= velx;
		collision_box.x = creaper_rect.x + 37;
	}

	creaper_rect.y += vely;
	collision_box.y = creaper_rect.y + 43;
	if (collision_box.y < 0 || collision_box.y + 55 > LEVEL1_HEIGHT || touch_wall(collision_box, tiles) || check_collision(boss_rect, collision_box))
	{
		if (check_collision(collision_box, boss_rect))
		{
			if (delay_damage_when_creaper_get_damage())
			{
				BLOOD::reduce_blood_by_damge(BOSS_TOUCH_DAMAGE);
				if (BLOOD::get_remain_blood() <= 0) is_creaper_alive = false;
			}
		}
		creaper_rect.y -= vely;
		collision_box.y = creaper_rect.y + 43;
	}
}
void Creaper::set_camera(SDL_Rect& camera)
{
	camera.x = (creaper_rect.x + CREAPER_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (creaper_rect.y + CREAPER_HEIGHT / 2) - SCREEN_HEIGHT / 2;
	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.x > LEVEL1_WIDTH - camera.w) camera.x = LEVEL1_WIDTH - camera.w;
	if (camera.y > LEVEL1_HEIGHT - camera.h) camera.y = LEVEL1_HEIGHT - camera.h;
}
bool Creaper::delay_damage_when_creaper_get_damage()
{
	if (SDL_GetTicks() - rotTime > 1000)
	{
		rotTime = SDL_GetTicks();
		return true;
	}
	return false;
}
void Creaper::render_creaper(SDL_Renderer* screen, SDL_Rect& camera)
{
	if (is_creaper_alive)
	{
		if (input_button.stand == true && BLOOD::get_remain_blood() > 0)
		{
			frame++;
			if (frame / 2 > Creaper_total_idle_frame) frame = 0;
			TEXTURE::render_texture(creaper_rect.x - camera.x, creaper_rect.y - camera.y, &Creaper_Idle_frame[frame / 2], screen, change_direction);
		}
		else if ((input_button.down_ || input_button.up_ || input_button.left_ || input_button.right_)&& BLOOD::get_remain_blood()>0)
		{
			frame++;
			if (frame / 2 > Creaper_total_runing_frame) frame = 0;
			TEXTURE::render_texture(creaper_rect.x - camera.x, creaper_rect.y - camera.y, &Creaper_Running_frame[frame / 2], screen, change_direction);
		}
	}
	else return;
}
void Creaper::handle_shuriken_list(SDL_Renderer* screen, SDL_Rect& camera, TILE* tiles[])
{
	if (SHURIKEN.size() >= 1)
	{
		for (int i = 0; i < SHURIKEN.size(); ++i)
		{
			if (SHURIKEN[i] != NULL)
			{
				if (SHURIKEN[i]->get_is_bullet_move())
				{
					SHURIKEN[i]->handle_bullet(tiles);
					SHURIKEN[i]->render_bullet(screen, camera);
				}
				else
				{
					SHURIKEN.erase(SHURIKEN.begin() + i);
				}
			}
		}
	}
	else return;
}
void Creaper::render_creaper_blood(SDL_Renderer* screen, SDL_Rect& camera)
{
	if (is_creaper_alive)
	{
		BLOOD::render_blood_bar(creaper_rect.x - camera.x + 10, creaper_rect.y - camera.y, screen);
	}
	else return;
}
void Creaper::handle_creaper_life(vector<BOSS_BULLET*> boss_bullet_list, vector<METEO*> boss_meteo_list, const int& boss_bullet_damage, const int& boss_lava_damage)
{
	if (boss_bullet_list.size() >= 1)
	{
		for (int i = 0; i < boss_bullet_list.size(); ++i)
		{
			if (check_collision(collision_box, boss_bullet_list[i]->Get_Boss_Bullet_Rect()))
			{
				BLOOD::reduce_blood_by_damge(boss_bullet_damage);
				if (BLOOD::get_remain_blood() <= 0) is_creaper_alive = false;
			}
		}
	}
	
	if (boss_meteo_list.size() >= 1)
	{
		for (int i = 0; i < boss_meteo_list.size(); ++i)
		{
			if (boss_meteo_list[i]->Get_Is_Create_Lava_Pool())
			{
				if (check_player_in_lava_pool_or_not(collision_box, boss_meteo_list[i]->Get_Lava_Center_x(), boss_meteo_list[i]->Get_Lava_Center_y()))
				{
					if (delay_damage_when_creaper_get_damage())
					{
						BLOOD::reduce_blood_by_damge(boss_lava_damage);
						if (BLOOD::get_remain_blood() <= 0) is_creaper_alive = false;
					}
				}
			}
		}
	}
}