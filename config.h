/*Imports*/
#include "push.c"

/* appearance and colors (minimalist as fuck)*/
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "JetBrainsMono:size=10:antialias=true:autohint=true" };
static const char dmenufont[]       = "JetBrainsMono:size=10:antialias=true:autohint=true";
static const char col_black[]       = "#171616";
static const char col_white[]       = "#F7F7F7";
static const char col_gray[]        = "#3D3D3D";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_black, col_gray },
	[SchemeSel]  = { col_white, col_gray, col_gray },
};

/* tagging in bar*/
/* (another bar)static const char *tags[] = { "α", "β", "γ", "δ", "ε", "ζ", "η", "θ", "ι" }; */
static const char *tags[] = { "term", "emacs", "www", "docs", "fm", "irc", "recs", "foo", "bar" };

/*rules for the layouts. i dont use it*/
static const Rule rules[] = {
	 /*class         instance    title       tags mask     isfloating   monitor*/
        { "Emacs",       NULL,       NULL,       1 << 1,       0,           -1 },
	{ "qutebrowser", NULL,       NULL,       1 << 2,       0,           -1 },	
	{ "Telegram",    NULL,       NULL,       1 << 5,       0,           -1 },
	{ "Ardour",      NULL,       NULL,       1 << 6,       0,           -1 },
	{ "Gimp",        NULL,       NULL,       1 << 6,       0,           -1 },
};

/* layouts */
static const float mfact     = 0.5;  /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[∀x]",     tile },    /* first entry is default */
	{ "[¬l]",     NULL },    /* no layout function means floating behavior */
	{ "[∃x]",     monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-p", ">>>", NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *fmcmd[]    = { "dmenufm", NULL };

/* Hardware keys for volume and brightness */
#include <X11/XF86keysym.h>
static const char *mutevol[] 			= { "volume", "--toggle",  NULL };
static const char *upvol[]   			= { "volume", "--inc",  	NULL };
static const char *downvol[] 			= { "volume", "--dec",    	NULL };
static const char *upbl[] 				= { "brightness", "--inc",    NULL };
static const char *downbl[] 			= { "brightness", "--dec",  NULL };

/* Screenshot */
static const char *shotnow[]  			= { "takeshot", "--now", NULL };

static Key keys[] = {

	/* modifier 				key 						function 		argument */

	// Hardware Keys -----------
	{ 0, 						XF86XK_AudioMute, 			spawn, {.v = mutevol } },
	{ 0, 						XF86XK_AudioLowerVolume, 	spawn, {.v = downvol } },
	{ 0, 						XF86XK_AudioRaiseVolume, 	spawn, {.v = upvol   } },
	{ 0, 						XF86XK_MonBrightnessUp, 	spawn, {.v = upbl   } },
	{ 0, 						XF86XK_MonBrightnessDown, 	spawn, {.v = downbl   } },

	// print
	{ 0, 						XK_Print, 					spawn, {.v = shotnow } },

	// terminal and dmenu
	{ MODKEY,                       XK_o,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },

	// change focus
    { MODKEY, 					XK_h, 						focusstack, {.i = +1 } }, // change window
    { MODKEY, 					XK_s, 						focusstack, {.i = -1 } },
    { MODKEY|ShiftMask, 		XK_t, 				restack, {.i = +1 } }, // change master
    { MODKEY|ShiftMask, 		XK_n, 				restack, {.i = -1 } },

	{ MODKEY|ShiftMask,             XK_Down,   pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_Up,     pushup,         {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } }, //vertical
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } }, //horizontal

	// resize
    { MODKEY|ControlMask, 		XK_Left, 					setmfact, {.f = -0.05} }, // Shrink left
    { MODKEY|ControlMask, 		XK_Right, 					setmfact, {.f = +0.05} }, // Shrink right

	// misc config
	{ MODKEY, 						XK_b, togglebar, 			{0} }, // Toggle bar
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },

	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	// layouts
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

	// TAGKEYS for virtual desktops
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
