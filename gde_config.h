#ifndef _GDE_CONFIG_H_
#define _GDE_CONFIG_H_

#include <stddef.h>
#include <stdint.h>

#include <linux/input-event-codes.h>
#include <xkbcommon/xkbcommon.h>

#include <wlr/types/wlr_keyboard.h>

//{{{ C "interface" so to speak

typedef union {
  int i;
  uint32_t ui;
  float f;
  const void *v;
} Arg;

typedef struct {
  uint32_t mod;
  xkb_keysym_t keysym;
  void (*func)(const Arg *);
  const Arg arg;
} Key;

typedef struct {
  unsigned int mod;
  unsigned int button;
  void (*func)(const Arg *);
  const Arg arg;
} Button;

enum { CurNormal, CurPressed, CurMove, CurResize }; /* cursor */

extern void chvt(const Arg *arg);
extern void incnmaster(const Arg *arg);
extern void killclient(const Arg *arg);
extern void focusstack(const Arg *arg);
extern void moveresize(const Arg *arg);
extern void quit(const Arg *arg);
extern void setmfact(const Arg *arg);
extern void spawn(const Arg *arg);
extern void tag(const Arg *arg);
extern void togglefloating(const Arg *arg);
extern void toggletag(const Arg *arg);
extern void toggleview(const Arg *arg);
extern void view(const Arg *arg);

//}}}

//{{{ Settings
//
/* appearance */
static const int sloppyfocus = 1; /* focus follows mouse */
static const int bypass_surface_visibility =
    0; /* 1 means idle inhibitors will disable idle tracking even if it's
          surface isn't visible  */
static const unsigned int borderpx = 2; /* border pixel of windows */
static const float bordercolor[] = {0.3, 0.3, 0.3, 1.0};
static const float focuscolor[] = {0.5, 0.0, 1.0, 1.0};
/* To conform the xdg-protocol, set the alpha to zero to restore the old
 * behavior */
static const float fullscreen_bg[] = {0.1, 0.1, 0.1, 1.0};

/* tagging - tagcount must be no greater than 31 */
static const int tagcount = 12;

// New clients attach at the bottom of the stack instead of at the top
static const int attach_bottom = 1;

// Mouse warps to center of newly-focused client
static const int cursor_warp = 1;

// Keyboard configuration to load
// TODO: make this resolve ~ or $HOME...
static const char *keyboard_xkb_path = "/home/g/.config/gdk/keyboard.xkb";

//}}}

//{{{ Key binds

#define MOD_S WLR_MODIFIER_SHIFT
#define MOD_A WLR_MODIFIER_ALT
#define MOD_M WLR_MODIFIER_LOGO
#define MOD_C WLR_MODIFIER_CTRL

#define MODKEY MOD_M
#define MODHYPER (MOD_M | MOD_A | MOD_C | MOD_S)

#define TAGKEYS(KEY, SKEY, TAG)                                                \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | MOD_C, KEY, toggleview, {.ui = 1 << TAG}},                     \
      {MODKEY | MOD_S, SKEY, tag, {.ui = 1 << TAG}}, {                         \
    MODKEY | MOD_C | MOD_S, SKEY, toggletag, { .ui = 1 << TAG }                \
  }

/* spawn commands */
static const char *termcmd[] = {"alacritty", "-e", "tmux", NULL};
static const char *menucmd[] = {"wofi", "--show=drun", "--allow-images", NULL};

static const Key keys[] = {
    /* Note that Shift changes certain key codes: c -> C, 2 -> at, etc. */
    /* modifier                  key                 function        argument */
    {MODKEY, XKB_KEY_o, spawn, {.v = menucmd}},
    {MODKEY, XKB_KEY_Return, spawn, {.v = termcmd}},
    {MODKEY, XKB_KEY_Down, focusstack, {.i = +1}},
    {MODKEY, XKB_KEY_Up, focusstack, {.i = -1}},
    {MODKEY, XKB_KEY_comma, incnmaster, {.i = +1}},
    {MODKEY, XKB_KEY_period, incnmaster, {.i = -1}},
    {MODKEY, XKB_KEY_Left, setmfact, {.f = -0.05}},
    {MODKEY, XKB_KEY_Right, setmfact, {.f = +0.05}},
    {MODKEY, XKB_KEY_Escape, togglefloating, {0}},

    TAGKEYS(XKB_KEY_1, XKB_KEY_exclam, 0),
    TAGKEYS(XKB_KEY_2, XKB_KEY_at, 1),
    TAGKEYS(XKB_KEY_3, XKB_KEY_numbersign, 2),
    TAGKEYS(XKB_KEY_4, XKB_KEY_dollar, 3),
    TAGKEYS(XKB_KEY_5, XKB_KEY_percent, 4),
    TAGKEYS(XKB_KEY_6, XKB_KEY_asciicircum, 5),
    TAGKEYS(XKB_KEY_7, XKB_KEY_ampersand, 6),
    TAGKEYS(XKB_KEY_8, XKB_KEY_asterisk, 7),
    TAGKEYS(XKB_KEY_9, XKB_KEY_parenleft, 8),
    TAGKEYS(XKB_KEY_0, XKB_KEY_parenright, 9),

    {WLR_MODIFIER_CTRL, XKB_KEY_q, killclient, {0}},
    {MODHYPER, XKB_KEY_Q, quit, {0}},

    /* Ctrl-Alt-Backspace and Ctrl-Alt-Fx used to be handled by X server */
    {WLR_MODIFIER_CTRL | WLR_MODIFIER_ALT, XKB_KEY_Terminate_Server, quit, {0}},

// CHange Virtual TTY
#define CHVT(n)                                                                \
  {                                                                            \
    MOD_C | MOD_A, XKB_KEY_XF86Switch_VT_##n, chvt, { .ui = (n) }              \
  }
    CHVT(1),
    CHVT(2),
    CHVT(3),
    CHVT(4),
    CHVT(5),
    CHVT(6),
    CHVT(7),
    CHVT(8),
    CHVT(9),
    CHVT(10),
    CHVT(11),
    CHVT(12),
};

//}}}

//{{{ Mouse buttons

static const Button buttons[] = {
    {MODKEY, BTN_LEFT, moveresize, {.ui = CurMove}},
    {MODKEY, BTN_MIDDLE, togglefloating, {0}},
    {MODKEY, BTN_RIGHT, moveresize, {.ui = CurResize}},
};

//}}}

#endif // _GDE_CONFIG_H_
