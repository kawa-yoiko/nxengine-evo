
#include "pause.h"

#include "../autogen/sprites.h"
#include "../game.h"
#include "../graphics/font.h"
#include "../graphics/graphics.h"
#include "../graphics/sprites.h"
#include "../input.h"
#include "../nx.h"
#include "../screeneffect.h"
#include "../endgame/credits.h"
#include "../inventory.h"
#include "../map_system.h"
using namespace Graphics;
using namespace Sprites;
#include "dialog.h"
using namespace Options;

Dialog *dlg;
unsigned int mm_cursel;

void _resume(ODItem *item, int dir)
{
  lastinputs[F1KEY] = true;
  game.pause(false);
}

void _options(ODItem *item, int dir)
{
  game.pause(GP_OPTIONS);
}

void _reset(ODItem *item, int dir)
{
  lastinputs[F2KEY] = true;
  game.reset();
}

void _exit(ODItem *item, int dir)
{
  lastinputs[ESCKEY] = true;
  game.running       = false;
}

bool pause_init(int param)
{
  memset(lastinputs, 1, sizeof(lastinputs));
  dlg = new Dialog();
  dlg->AddItem("Resume", _resume);
  dlg->AddItem("Options", _options);
  dlg->AddItem("Reset", _reset);
  dlg->AddItem("Quit", _exit);
  dlg->SetSelection(mm_cursel);
  dlg->ShowFull();
  int maxsize = 0;
  for (auto &item : dlg->Items())
  {
    int x = GetFontWidth(_(item->text));
    if (x > maxsize)
      maxsize = x;
  }
  dlg->SetSize(maxsize + 60, 70);
  return 0;
}

void pause_tick()
{
  DrawScene();
  if (game.mode == GM_NORMAL)
    DrawStatusBar();
  if (game.mode == GM_INVENTORY)
    DrawInventory();
  if (game.mode == GM_MAP_SYSTEM)
    ms_draw();

  fade.Draw();

  textbox.Draw();

  if (game.mode == GM_CREDITS)
  {
    credit_draw();
  }

  Graphics::TintScreen();

  dlg->RunInput();
  dlg->Draw();

  if (justpushed(ESCKEY))
  {
    _resume(NULL, 0);
    return;
  }
}
