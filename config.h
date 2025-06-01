/* See LICENSE file for copyright and license details. */

void togglefullscreen(const Arg *arg)
{
	if (selmon->sel)
		setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
}

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 8;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_bg_nor[]       = "#1c1c1c";
static const char col_bg_sel[]       = "#4d4d4d";
static const char col_fg_nor[]       = "#cccccc";
static const char col_fg_sel[]       = "#eeeeee";
static const char *colors[][3]      = {
	/*               fg          bg          border   */
	[SchemeNorm] = { col_fg_nor, col_bg_nor, "#000000" },
	[SchemeSel]  = { col_fg_sel, col_bg_sel, col_bg_sel },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4" };

static const Rule rules[] = {
	/* xprop(1):
	 *   WM_CLASS(STRING) = instance, class
	 *   WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define DMENU_ARGS "-fn", dmenufont, "-nb", col_bg_nor, "-nf", col_fg_nor, "-sb", col_bg_sel, "-sf", col_fg_sel

/* commands */
static const char *cmd_dmenu[]   = { "dmenu_run", DMENU_ARGS, NULL };
static const char *cmd_filesel[] = { "openmenu", DMENU_ARGS, NULL };
static const char *cmd_readdoc[] = { "surfdoc", DMENU_ARGS, NULL };
static const char *cmd_term[]    = { "xfce4-terminal", NULL };
static const char *cmd_lock[]    = { "lock", NULL };
static const char *cmd_scrshot[]     = { "scr", NULL };
static const char *cmd_scrshotsel[]  = { "scr", "-s", NULL };
static const char *cmd_audio_up[]    = { "media", "vol+", NULL };
static const char *cmd_audio_down[]  = { "media", "vol-", NULL };
static const char *cmd_audio_mute[]  = { "media", "mute", NULL };
static const char *cmd_media_next[]  = { "media", "next", NULL };
static const char *cmd_media_prev[]  = { "media", "prev", NULL };
static const char *cmd_media_pause[] = { "media", "pause", NULL };
static const char *cmd_media_stop[]  = { "media", "stop", NULL };
static const char *cmd_bright_up[]   = { "brightnessctl", "set", "5%+", NULL };
static const char *cmd_bright_down[] = { "brightnessctl", "set", "5%-", NULL };

#include <X11/XF86keysym.h>

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = cmd_dmenu } },
	{ MODKEY,                       XK_o,      spawn,          {.v = cmd_filesel } },
	{ MODKEY|ShiftMask,             XK_o,      spawn,          {.v = cmd_readdoc } },
	{ MODKEY,                       XK_ccedilla, spawn,        SHCMD("ccedilla") },
	{ MODKEY,                       XK_c,      spawn,          {.v = cmd_term } },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = cmd_lock } },
	{ MODKEY,                       XK_p,      spawn,          {.v = cmd_scrshot } },
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = cmd_scrshotsel } },
	{ MODKEY,                       XK_equal,  spawn,          {.v = cmd_audio_up} },
	{ MODKEY,                       XK_minus,  spawn,          {.v = cmd_audio_down} },
	{ MODKEY,                       XK_BackSpace,    spawn,    {.v = cmd_audio_mute} },
	{ MODKEY,                       XK_bracketright, spawn,    {.v = cmd_media_next} },
	{ MODKEY,                       XK_bracketleft,  spawn,    {.v = cmd_media_prev} },
	{ MODKEY,                       XK_Return, spawn,          {.v = cmd_media_pause} },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = cmd_media_stop} },
	{ 0, XF86XK_AudioRaiseVolume,              spawn,          {.v = cmd_audio_up} },
	{ 0, XF86XK_AudioLowerVolume,              spawn,          {.v = cmd_audio_down} },
	{ 0, XF86XK_AudioMute,                     spawn,          {.v = cmd_audio_mute} },
	{ 0, XF86XK_AudioNext,                     spawn,          {.v = cmd_media_next} },
	{ 0, XF86XK_AudioPrev,                     spawn,          {.v = cmd_media_prev} },
	{ 0, XF86XK_AudioPause,                    spawn,          {.v = cmd_media_pause} },
	{ 0, XF86XK_AudioStop,                     spawn,          {.v = cmd_media_stop} },
	{ 0, XF86XK_MonBrightnessUp,               spawn,          {.v = cmd_bright_up } },
	{ 0, XF86XK_MonBrightnessDown,             spawn,          {.v = cmd_bright_down } },
	{ MODKEY,                       XK_f,      togglefullscreen, {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_F8,     incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_F8,     incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.025} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.025} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_F1,     setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_F2,     setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_F3,     setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_s,      togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0),
	TAGKEYS(                        XK_2,                      1),
	TAGKEYS(                        XK_3,                      2),
	TAGKEYS(                        XK_4,                      3),
	{ MODKEY|ShiftMask,             XK_Delete, quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkWinTitle,          0,              Button5,        focusstack,     {.i = +1 } },
	{ ClkWinTitle,          0,              Button4,        focusstack,     {.i = -1 } },
	{ ClkStatusText,        0,              Button1,        spawn,          {.v = cmd_term } },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = cmd_lock } },
	{ ClkStatusText,        0,              Button4,        spawn,          {.v = cmd_audio_up } },
	{ ClkStatusText,        0,              Button5,        spawn,          {.v = cmd_audio_down } },
	{ ClkStatusText,        0,              Button3,        spawn,          {.v = cmd_audio_mute } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
