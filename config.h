/* See LICENSE file for copyright and license details. */

static void togglefullscr(const Arg *arg);

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 4;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=9" };
static const char dmenufont[]       = "monospace:size=9";
static const char col_bg_norm[] = "#151515";
static const char col_bg_sel[]  = "#151515";
static const char col_fg_norm[] = "#525250";
static const char col_fg_sel[]  = "#e8e8d3";
static const char col_br_norm[] = "#1b1b1b";
static const char col_br_sel[]  = "#525250";
static const char *colors[][3]  = {
	/*               fg           bg           border   */
	[SchemeNorm] = { col_fg_norm, col_bg_norm, col_br_norm },
	[SchemeSel]  = { col_fg_sel,  col_bg_sel,  col_br_sel  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
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
#define CMD(...)   { .v = (const char*[]){ __VA_ARGS__, NULL } }
#define SHCMD(...) { .v = (const char*[]){ "/bin/sh", "-c", __VA_ARGS__, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bg_norm, "-sb", col_br_sel, "-sf", col_fg_sel, NULL };

#include <X11/XF86keysym.h>

#include "movestack.h"

static const Key keys[] = {
	/* modifier                    key          function        argument */
	{ MODKEY,                      XK_d,        spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,            XK_d,        spawn,          CMD("dmenu-sh-run") },
	{ MODKEY|ControlMask,          XK_d,        spawn,          CMD("dmenu-sudo-run") },
	{ MODKEY|Mod1Mask,             XK_d,        spawn,          CMD("dmenu-unicode") },
	{ MODKEY,                      XK_c,        spawn,          SHCMD("command -V tmux && exec ${TERMINAL:-st} -e tmux || exec ${TERMINUAL:-st}") },
	{ MODKEY|ShiftMask,            XK_c,        spawn,          SHCMD("${TERMINAL:-st}") },
	{ MODKEY,                      XK_n,        togglebar,      {0} },
	{ MODKEY,                      XK_j,        focusstack,     {.i = +1 } },
	{ MODKEY,                      XK_k,        focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,            XK_j,        movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,            XK_k,        movestack,      {.i = -1 } },
	{ MODKEY,                      XK_i,        incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,            XK_i,        incnmaster,     {.i = -1 } },
	{ MODKEY,                      XK_h,        setmfact,       {.f = -0.05} },
	{ MODKEY,                      XK_l,        setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,            XK_h,        setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,            XK_l,        setcfact,       {.f = +0.25} },
	{ MODKEY|ControlMask,          XK_h,        setcfact,       {.f = 0} },
	{ MODKEY|ControlMask,          XK_l,        setcfact,       {.f = 0} },
	{ MODKEY,                      XK_space,    zoom,           {0} },
	{ MODKEY,                      XK_Tab,      view,           {0} },
	{ MODKEY,                      XK_q,        killclient,     {0} },
	{ MODKEY,                      XK_F1,       setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                      XK_F2,       setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                      XK_F3,       setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                      XK_s,        togglefloating, {0} },
	{ MODKEY,                      XK_0,        view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,            XK_0,        tag,            {.ui = ~0 } },
	{ MODKEY,                      XK_comma,    focusmon,       {.i = -1 } },
	{ MODKEY,                      XK_period,   focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,            XK_comma,    tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,            XK_period,   tagmon,         {.i = +1 } },
	{ MODKEY,                      XK_f,        togglefullscr,  {0} },
	TAGKEYS(                       XK_1,                        0),
	TAGKEYS(                       XK_2,                        1),
	TAGKEYS(                       XK_3,                        2),
	TAGKEYS(                       XK_4,                        3),
	TAGKEYS(                       XK_5,                        4),
	TAGKEYS(                       XK_6,                        5),
	TAGKEYS(                       XK_7,                        6),
	TAGKEYS(                       XK_8,                        7),
	TAGKEYS(                       XK_9,                        8),
	{ MODKEY,                      XK_Delete,   quit,           {0} },
	{ MODKEY,                      XK_b,        spawn,          SHCMD("${BROWSER:-firefox}") },
	{ MODKEY|ShiftMask,            XK_b,        spawn,          SHCMD("${CHROME_BROWSER:-chromium}") },
	{ MODKEY,                      XK_x,        spawn,          SHCMD("${COLOR_PICKER:-gpick}") },
	{ MODKEY|ShiftMask,            XK_e,        spawn,          SHCMD("${TERMINAL:-st} -e ${EDITOR:-nano}") },
	{ MODKEY,                      XK_e,        spawn,          SHCMD("${EXPLORER:-pcmanfm}") },
	{ MODKEY,                      XK_g,        spawn,          SHCMD("${IMAGE_EDITOR:-gimp --no-splash}") },
	{ MODKEY|ShiftMask,            XK_x,        spawn,          SHCMD("${METRONOME:-gtick}") },
	{ MODKEY,                      XK_t,        spawn,          SHCMD("${TASK_MANAGER:-${TERMINAL:-st} -e htop}") },
	{ MODKEY,                      XK_ccedilla, spawn,          SHCMD("${CALCULATOR:-${TERMINAL:-st} -e qalc}") },
	{ MODKEY,                      XK_v,        spawn,          SHCMD("${VISUAL:-emacs}") },
	{ MODKEY|ShiftMask,            XK_v,        spawn,          SHCMD("${WORD_PROCESSOR:-libreoffice --nologo --writer}") },
	{ MODKEY,                      XK_m,        spawn,          SHCMD("${MIXER:-${TERMINAL:-st} -e pulsemixer}") },
	{ MODKEY,                      XK_p,        spawn,          CMD("set-monitor-layout") },
	{ MODKEY|ShiftMask,            XK_p,        spawn,          SHCMD("${DISPLAY_CONFIGURATOR:-arandr}") },
	{ MODKEY,                      XK_r,        spawn,          CMD("remap") },
	{ MODKEY,                      XK_u,        spawn,          SHCMD("${SCREEN_LOCKER:-slock}") },
	{ MODKEY|ShiftMask,            XK_u,        spawn,          SHCMD("${SCREEN_LOCKER:-slock} & sleep 1 && exec ${SYSTEM_SUSPEND_COMMAND:-systemctl suspend}") },
	{ MODKEY,                      XK_w,        spawn,          SHCMD("${TERMINAL:-st} -e nmtui") },
	{ 0,                           XK_Print,    spawn,          CMD("printscreen") },
	{ ShiftMask,                   XK_Print,    spawn,          CMD("printscreen", "-s") },
	{ ControlMask,                 XK_Print,    spawn,          CMD("printscreen", "-i") },
	{ MODKEY, XK_BackSpace,                     spawn,          CMD("pamixer", "-t") },
	{ MODKEY, XK_minus,                         spawn,          CMD("pamixer", "--allow-boost", "-d", "3") },
	{ MODKEY, XK_equal,                         spawn,          CMD("pamixer", "--allow-boost", "-i", "3") },
	{ MODKEY, XK_Return,                        spawn,          CMD("playerctl", "play-pause") },
	{ MODKEY, XK_bracketright,                  spawn,          CMD("playerctl", "next") },
	{ MODKEY, XK_bracketleft,                   spawn,          CMD("playerctl", "previous") },
	{ 0, XF86XK_AudioMute,                      spawn,          CMD("pamixer", "-t") },
	{ 0, XF86XK_AudioLowerVolume,               spawn,          CMD("pamixer", "--allow-boost", "-d", "3") },
	{ 0, XF86XK_AudioRaiseVolume,               spawn,          CMD("pamixer", "--allow-boost", "-i", "3") },
	{ 0, XF86XK_AudioPlay,                      spawn,          CMD("playerctl", "play-pause") },
	{ 0, XF86XK_AudioNext,                      spawn,          CMD("playerctl", "next") },
	{ 0, XF86XK_AudioPrev,                      spawn,          CMD("playerctl", "previous") },
	{ 0, XF86XK_MonBrightnessDown,              spawn,          CMD("brightnessctl", "s", "5%-") },
	{ 0, XF86XK_MonBrightnessUp,                spawn,          CMD("brightnessctl", "s", "5%+") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        spawn,          CMD("dwm-status") },
	{ ClkStatusText,        0,              Button2,        spawn,          SHCMD("${TERMINAL:-st}") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

void
togglefullscr(const Arg *arg) {
	if (selmon->sel)
		setfullscreen(selmon->sel, !selmon->sel->isfullscreen);
}
