/* appearance */
static const double activeopacity   = 1.0f;     /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity = 1.0f;   /* Window opacity when it's inactive (0 <= opacity <= 1) */
static       Bool bUseOpacity       = True;     /* Starts with opacity on any unfocused windows */
static int user_bh            = 18;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static unsigned int borderpx  = 2;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static unsigned int gappih    = 8;       /* horiz inner gap between windows */
static unsigned int gappiv    = 8;       /* vert inner gap between windows */
static unsigned int gappoh    = 8;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 8;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static int showsystray        = 1;     /* 0 means no systray */
static int showbar            = 1;     /* 0 means no bar */
static int topbar             = 1;     /* 0 means bottom bar */
#define ICONSIZE 16   /* icon size */
#define ICONSPACING 5 /* space between icon and title */
static const char *fonts[]          = { "Droid Sans:pixelsize=15:antialias=true:autohint=true",
        "Hack Nerd Font:pixelsize=15:antialias=true:autohint=true",
        "Noto Color Emoji:pixelsize=13:antialias=true:autohint=true" };
static char font[]            = "monospace:size=10";
static char dmenufont[]       = "Droid Sans:pixelsize=14:antialias=true:autohint=true";
static char normbgcolor[]           = "#0d1321";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
//static char selbordercolor[]        = "#696501";
static char selbgcolor[]            = "#535c00";
static char act_bor[]               = "#edeae1";
static char inact_bor[]             = "#141001";
static char urg_bor[]               = "#ff1100";
static char *colors[][3]      = {
	/*               fg         bg         border  */
        [SchemeNorm] = { normfgcolor, normbgcolor, inact_bor },
        [SchemeSel]  = { selfgcolor,  selbgcolor,  act_bor  },
        [SchemeUrg]  = { selfgcolor,  selbgcolor,  urg_bor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
//static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

static const XPoint stickyicon[]    = { {0,0}, {4,0}, {4,8}, {2,6}, {0,8}, {0,0} }; /* represents the icon as an array of vertices */
static const XPoint stickyiconbb    = {4,8};	/* defines the bottom right corner of the polygon's bounding box (speeds up scaling) */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "galculator",  NULL,     NULL,       0,         1,          0,           0,        -1 },
	{ "firefox", NULL,     NULL,           0,         0,          0,          -1,        -1 },
	{ "st",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "alacritty",   NULL,  NULL,          0,         0,          1,           0,        -1 },
	{ "urxvt",   NULL,      NULL,          0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
    { NULL,     "spterm",     NULL,       SPTAG(0),   1,          0,           1,        -1 },
    { NULL,     "spmp",       NULL,       SPTAG(1),   1,          0,           1,        -1 },
    { NULL,     "galculator",  NULL,      SPTAG(2),   1,          0,           1,        -1 },
};

static const unsigned int ulinepad  = 5;    /* horizontal padding between the underline and tag */
static const unsigned int ulinestroke  = 1;    /* thickness / height of the underline */
static const unsigned int ulinevoffset = 0;    /* how far above the bottom of the bar the line should appear */
static const int ulineall      = 0;    /* 1 to show underline on all tags, 0 for just the active ones */

typedef struct {
    const char *name;
    const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "135x35", NULL };
const char *spcmd2[] = {"st", "-n", "spmp", "-g", "135x35", "-e", "ncmpcpp", NULL };
const char *spcmd3[] = {"galculator", NULL };
static Sp scratchpads[] = {
    /* name          cmd  */
    {"spterm",      spcmd1},
    {"spmp",        spcmd2},
    {"galculator",  spcmd3},
};

/* layout(s) */
static float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int decorhints  = 1;    /* 1 means respect decoration hints */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define STATUSBAR "dwmblocks"

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#include "shift-tools.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "TTT",      bstack },

    { "[@]",      spiral },
    { "[\\]",     dwindle },

    { "[M]",      monocle },
    { "H[]",      deck },

    { "|M|",      centeredmaster },
    { ">M>",      centeredfloatingmaster },

    { "HHH",      grid },
    { "><>",      NULL },    /* no layout function means floating behavior */
    { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \
         { ControlMask,                       KEY,      focusnthmon,    {.i  = TAG } }, \
         { ControlMask|ShiftMask,             KEY,      tagnthmon,      {.i  = TAG } },

#define STACKKEYS(MOD,ACTION) \
    { MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
    { MOD, XK_k,     ACTION##stack, {.i = INC(-1) } }, \
    { MOD, XK_backslash,     ACTION##stack, {.i = 0 } }, \
/*    { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
    { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
    { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
    { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

#include <X11/XF86keysym.h>

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

//Constants
#define TERMINAL "st"
#define BROWSER "$BROWSER"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
        { "font",               STRING,  &font },
        { "user_bh",            STRING,  &user_bh },
        { "gappih",             INTEGER,  &gappih },
        { "gappiv",             INTEGER,  &gappiv },
        { "gappoh",             INTEGER,  &gappoh },
        { "gappov",             INTEGER,  &gappov },
        { "showsystray",        INTEGER,  &showsystray },
        { "act_bor",            STRING,  &act_bor },
        { "inact_bor",          STRING,  &inact_bor },
        { "urg_bor",            STRING,  &urg_bor },
        { "dmenufont",          STRING,  &dmenufont },
        { "normbgcolor",        STRING,  &normbgcolor },
        { "normbordercolor",    STRING,  &normbordercolor },
        { "normfgcolor",        STRING,  &normfgcolor },
        { "selbgcolor",         STRING,  &selbgcolor },
       // { "selbordercolor",     STRING,  &selbordercolor },
        { "selfgcolor",         STRING,  &selfgcolor },
        { "borderpx",           INTEGER, &borderpx },
        { "snap",               INTEGER, &snap },
        { "showbar",            INTEGER, &showbar },
        { "topbar",             INTEGER, &topbar },
        { "nmaster",            INTEGER, &nmaster },
        { "resizehints",        INTEGER, &resizehints },
        { "mfact",              FLOAT,   &mfact },
};

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },

	{ MODKEY,                       XK_b,      togglebar,      {0} },
    STACKKEYS(MODKEY,                          focus)
    STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY,                       XK_p,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_o,      incnmaster,     {.i = -1 } },
    { MODKEY|ShiftMask,             XK_p,      resetnmaster,    {0} },
    { MODKEY,                       XK_g,      reorganizetags, {0} },
    { MODKEY,                       XK_s,      togglesticky,   {0} },
    { MODKEY,                       XK_z,      transfer,       {0} },
    { MODKEY|ShiftMask,             XK_z,      transferall,    {0} },

	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
    { MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
    { MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
    { MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },

    { MODKEY,                       XK_equal,  incrgaps,       {.i = +1 } },
    { MODKEY,                       XK_minus,  incrgaps,       {.i = -1 } },
/*  { MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } },
    { MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
    { MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } },
    { MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
    { MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } },
    { MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
    { MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } },
    { MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
    { MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } },
    { MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
    { MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } },
    { MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },*/
    { MODKEY|ShiftMask,             XK_equal,  togglegaps,     {0} },
    { MODKEY|ShiftMask,             XK_minus,  defaultgaps,    {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },//tile
    { MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[1]} },//bottom tile
    { MODKEY,                       XK_y,      setlayout,      {.v = &layouts[2]} },//spindle
    { MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[3]} },//dwindle
    { MODKEY,                       XK_r,      setlayout,      {.v = &layouts[4]} },//monocle
    { MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[5]} },//deck
    { MODKEY,                       XK_u,      setlayout,      {.v = &layouts[7]} },//centered Master floating
    { MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[6]} },//centered Master
    { MODKEY,                       XK_i,      setlayout,      {.v = &layouts[8]} },//grid
    { MODKEY|ShiftMask,             XK_i,      setlayout,      {.v = &layouts[9]} },//floating
    { MODKEY,                       XK_Tab,    cyclelayout,    {.i = -1 } },
    { MODKEY|ShiftMask,             XK_Tab,    cyclelayout,    {.i = +1 } },

    { MODKEY|ControlMask,           XK_bracketleft,      shifttag,      { .i = -1 } },
    { MODKEY|ControlMask,           XK_bracketright,     shifttag,      { .i = +1 } },
    { MODKEY,                       XK_bracketleft,      shiftview,      { .i = -1 } },
    { MODKEY,                       XK_bracketright,     shiftview,      { .i = +1 } },
    { MODKEY|ShiftMask,             XK_bracketright,     shifttagclients,      { .i = +1 } },
    { MODKEY|ShiftMask,             XK_bracketleft,      shifttagclients,      { .i = -1 } },
    { MODKEY,                       XK_x,                unfloatvisible, {0} },

	{ MODKEY|ControlMask,           XK_Return, zoom,           {0} },
	{ MODKEY|ControlMask,           XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_a,      toggleopacity,  {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
//	{ MODKEY,                       XK_f,      setlayout,      {0} },
	{ MODKEY,                       XK_f,      togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
    { MODKEY|ShiftMask,             XK_space,  togglealwaysontop, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

    { MODKEY,                       XK_Page_Down, scratchpad_show, {0} },
    { MODKEY,                       XK_Page_Up, scratchpad_hide, {0} },
    { MODKEY,                       XK_Home,    scratchpad_remove,{0} },
    { MODKEY|ShiftMask,             XK_Return, togglescratch,  {.ui = 0 } },
    { Mod1Mask|ShiftMask,           XK_Return, togglescratch,  {.ui = 1 } },
    { ControlMask|ShiftMask,        XK_Return, togglescratch,  {.ui = 2 } },

    //Resize the windows with arrow keys
    { MODKEY,                       XK_Down,   moveresize,     {.v = "0x 25y 0w 0h" } },
    { MODKEY,                       XK_Up,     moveresize,     {.v = "0x -25y 0w 0h" } },
    { MODKEY,                       XK_Right,  moveresize,     {.v = "25x 0y 0w 0h" } },
    { MODKEY,                       XK_Left,   moveresize,     {.v = "-25x 0y 0w 0h" } },
    { MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
    { MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
    { MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
    { MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },
    { MODKEY|ControlMask,           XK_Up,     moveresizeedge, {.v = "t"} },
    { MODKEY|ControlMask,           XK_Down,   moveresizeedge, {.v = "b"} },
    { MODKEY|ControlMask,           XK_Left,   moveresizeedge, {.v = "l"} },
    { MODKEY|ControlMask,           XK_Right,  moveresizeedge, {.v = "r"} },
    { MODKEY|ControlMask|ShiftMask, XK_Up,     moveresizeedge, {.v = "T"} },
    { MODKEY|ControlMask|ShiftMask, XK_Down,   moveresizeedge, {.v = "B"} },
    { MODKEY|ControlMask|ShiftMask, XK_Left,   moveresizeedge, {.v = "L"} },
    { MODKEY|ControlMask|ShiftMask, XK_Right,  moveresizeedge, {.v = "R"} },

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
	{ MODKEY|ControlMask,           XK_q,      quit,           {1} },

    { Mod1Mask|ControlMask,         XK_v,               spawn,      SHCMD("pavucontrol") },
    { MODKEY,                       XK_n,               spawn,      SHCMD(TERMINAL " -e newsboat") },
    { MODKEY|ShiftMask,             XK_x,               spawn,      SHCMD("power") },
    { MODKEY|ShiftMask,             XK_e,               spawn,      SHCMD(TERMINAL " -e gotop") },
    { Mod1Mask|ShiftMask,           XK_s,               spawn,      SHCMD("maim -s ~/Data/screenshots/$(date +%Y-%m-%d-%s).png") },
    { Mod1Mask,                     XK_s,               spawn,      SHCMD("maim ~/Data/screenshots/$(date +%Y-%m-%d-%s).png") },
    { 0, XF86XK_AudioMute,                              spawn,      SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
    { 0, XF86XK_AudioRaiseVolume,                       spawn,      SHCMD("pamixer --allow-boost -i 3; pkill -RTMIN+10 dwmblocks") },
    { 0, XF86XK_AudioLowerVolume,                       spawn,      SHCMD("pamixer --allow-boost -d 3; pkill -RTMIN+10 dwmblocks") },
    { 0, XF86XK_MonBrightnessUp,                        spawn,      SHCMD("xbacklight -inc 5%; pkill -RTMIN+11 dwmblocks") },
    { 0, XF86XK_MonBrightnessDown,                      spawn,      SHCMD("xbacklight -dec 5%; pkill -RTMIN+11 dwmblocks") },
    { 0, XF86XK_AudioPrev,                              spawn,      SHCMD("mpc prev; pkill -RTMIN+12 dwmblocks") },
    { 0, XF86XK_AudioNext,                              spawn,      SHCMD("mpc next; pkill -RTMIN+12 dwmblocks") },
    { 0, XF86XK_AudioPlay,                              spawn,      SHCMD("mpc toggle; pkill -RTMIN+12 dwmblocks") },
    { 0, XF86XK_HomePage,                               spawn,      SHCMD("librewolf") },
    { Mod1Mask,                     XK_Up,              spawn,      SHCMD("pamixer --allow-boost -i 3; pkill -RTMIN+10 dwmblocks") },
    { Mod1Mask,                     XK_Down,            spawn,      SHCMD("pamixer --allow-boost -d 3; pkill -RTMIN+10 dwmblocks") },
    { Mod1Mask|ShiftMask,           XK_space,           spawn,      SHCMD("mpc toggle; pkill -RTMIN+12 dwmblocks") },
    { Mod1Mask|ShiftMask,           XK_Right,           spawn,      SHCMD("mpc next; pkill -RTMIN+12 dwmblocks") },
    { Mod1Mask|ShiftMask,           XK_Left,            spawn,      SHCMD("mpc prev; pkill -RTMIN+12 dwmblocks") },
    { Mod1Mask,                     XK_bracketleft,     spawn,      SHCMD("mpc seek -10") },
    { Mod1Mask|ShiftMask,           XK_bracketleft,     spawn,      SHCMD("mpc seek -60") },
    { Mod1Mask,                     XK_bracketright,    spawn,      SHCMD("mpc seek +10") },
    { Mod1Mask|ShiftMask,           XK_bracketright,    spawn,      SHCMD("mpc seek +60") },
    { Mod1Mask,                     XK_Left,            spawn,      SHCMD("mpc vol -5") },
    { Mod1Mask,                     XK_Right,           spawn,      SHCMD("mpc vol +5") },
    { Mod1Mask|ControlMask,         XK_space,           spawn,      SHCMD("mpc single") },
    { MODKEY|ShiftMask,             XK_w,               spawn,      SHCMD("set-wallpaper") },
    { MODKEY|ShiftMask,             XK_n,               spawn,      SHCMD(TERMINAL " -e newsboat") },
    { MODKEY|ShiftMask,             XK_a,               spawn,      SHCMD("rofi -show emoji") },
    { MODKEY,                       XK_w,               spawn,      SHCMD(BROWSER) },
    { MODKEY|ShiftMask,             XK_v,               spawn,      SHCMD("minitube") },
    { MODKEY,                       XK_e,               spawn,      SHCMD(TERMINAL " -e lf ") },
    { MODKEY|ShiftMask,             XK_m,               spawn,      SHCMD(TERMINAL " -e neomutt") },
    { MODKEY|ControlMask,           XK_e,               spawn,      SHCMD("pcmanfm") },
    { MODKEY,                       XK_v,               spawn,      SHCMD("transmission-gtk") },
    { MODKEY,                       XK_m,               spawn,      SHCMD("mailspring") },
    { MODKEY|ShiftMask,             XK_n,               spawn,      SHCMD(TERMINAL " -e vim") },
    { MODKEY|ControlMask,           XK_x,               spawn,      SHCMD("slock") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
    { ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

