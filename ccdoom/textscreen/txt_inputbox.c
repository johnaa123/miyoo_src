//
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "doomkeys.h"

#include "txt_inputbox.h"
#include "txt_gui.h"
#include "txt_io.h"
#include "txt_main.h"
#include "txt_utf8.h"
#include "txt_window.h"

extern txt_widget_class_t txt_inputbox_class;
extern txt_widget_class_t txt_int_inputbox_class;

#define USE_VIRTUALKEYBOARD
#ifdef USE_VIRTUALKEYBOARD
#include "txt_label.h"
#include "txt_window.h"
#include "txt_separator.h"
#include "txt_button.h"
#include "txt_strut.h"

//Forward Declarations, ugly warning suppressor
static void VKBCreateWindow(txt_inputbox_t *inputbox, int charset);
static void Backspace(txt_inputbox_t *inputbox);
static void AddCharacter(txt_inputbox_t *inputbox, int key);
static void FinishEditing(txt_inputbox_t *inputbox);

static int vkb_charset_index;
static txt_inputbox_t *vkb_inputbox;
static txt_label_t *vkb_label;
static txt_window_t *vkb_window;
static char *vkb_charset[] =
{
    "1234567890"
    "qwertyuiop"
    "asdfghjkl:"
    ",zxcvbnm/.",

    "1234567890"
    "QWERTYUIOP"
    "ASDFGHJKL;"
    "/ZXCVBNM<>",

    "1234567890"
    "!""#$%&'()+"
    "-/:;<=>?@*"
    "[\\]^_`{|}~",
};

static void VKBUpdateLabel()
{
    char buf[vkb_window->window_w - 5];

    if (strlen(vkb_inputbox->buffer) > vkb_window->window_w - 7)
    {
        sprintf(buf, "...%s",
                strchr(vkb_inputbox->buffer, '\0') - vkb_window->window_w - 10);
        TXT_SetLabel(vkb_label, buf);
    }
    else
    {
        TXT_SetLabel(vkb_label, vkb_inputbox->buffer);
    }
}

static void VKBAddCharacter(TXT_UNCAST_ARG(widget), int key)
{
    AddCharacter(vkb_inputbox, key);
    VKBUpdateLabel();
}

static void VKBBackspace(TXT_UNCAST_ARG(widget), void *userdata)
{
    Backspace(vkb_inputbox);
    VKBUpdateLabel();
}

static void VKBCycleCharsets(TXT_UNCAST_ARG(widget), void *userdata)
{
    TXT_CloseWindow(vkb_window);
    VKBCreateWindow(vkb_inputbox,
              (vkb_charset_index+1) %
              (sizeof(vkb_charset) / sizeof(vkb_charset[0])));
}

static void VKBCloseWindow(TXT_UNCAST_ARG(widget), int keep)
{
    if (keep)
    {
        FinishEditing(vkb_inputbox);
    }
    else
    {
        vkb_inputbox->editing = 0;
    }

    TXT_CloseWindow(vkb_window);
}

static void VKBCreateWindow(txt_inputbox_t *inputbox, int charset_index)
{
    int i;
    txt_table_t *keyboard;
    txt_table_t *softact;
    txt_table_t *softact_okcancel;
    txt_table_t *organizer;
    txt_window_action_t *cancel, *caps;

    //Create Widgets
    vkb_window = TXT_NewWindow(NULL);
    vkb_inputbox = inputbox;
    vkb_charset_index = charset_index;
    vkb_label = TXT_NewLabel("");
    keyboard = TXT_NewTable(19);
    organizer = TXT_NewTable(3);
    softact = TXT_NewTable(1);

    //Populate Window
    TXT_AddWidget(vkb_window, vkb_label);
    TXT_AddWidget(vkb_window, TXT_NewSeparator(NULL));
    TXT_AddWidget(vkb_window, organizer);
    TXT_AddWidget(organizer, keyboard);
    TXT_AddWidget(organizer, TXT_NewStrut(1, 0));
    TXT_AddWidget(organizer, softact);

    //Create Actions
    cancel = TXT_NewWindowAction(KEY_BBUTTON, "Revert");
    caps   = TXT_NewWindowAction(KEY_YBUTTON, "Caps");
    TXT_SignalConnect(cancel, "pressed", VKBCloseWindow,   0);
    TXT_SignalConnect(caps,   "pressed", VKBCycleCharsets, 0);
    TXT_AddWidget(softact, TXT_NewButton2("Backspace", VKBBackspace, NULL));
    TXT_AddWidget(softact, TXT_NewButton2("Caps-Lock", VKBCycleCharsets, NULL));
    TXT_AddWidget(softact, TXT_NewButton2("Revert", VKBCloseWindow, 0));
	TXT_AddWidget(softact, TXT_NewButton2("Accept", VKBCloseWindow, 1));

    //Create buttons and populate keyboard table
    for (i=0; vkb_charset[vkb_charset_index][i] != '\0'; i++)
    {
        char button[] = {vkb_charset[vkb_charset_index][i], '\0'};
        TXT_AddWidget(keyboard, TXT_NewButton2(button, VKBAddCharacter, button[0])); //, VKBAddCharacter));

        //Don't add padding on the last of row
        if ((i+1) % 10)
        {
            TXT_AddWidget(keyboard, TXT_NewStrut(1, 0));
        }
    }

    //Format & Update things
    TXT_SetWindowAction(vkb_window, TXT_HORIZ_LEFT,  cancel);
    TXT_SetWindowAction(vkb_window, TXT_HORIZ_RIGHT, caps);
    TXT_SetWidgetAlign(keyboard, TXT_HORIZ_CENTER);
    TXT_SetBGColor(vkb_label, TXT_COLOR_BLACK);
    TXT_LayoutWindow(vkb_window);
    VKBUpdateLabel();
	inputbox->editing = 1;
}

#endif // USE_VIRTUALKEYBOARD

static void SetBufferFromValue(txt_inputbox_t *inputbox)
{
    if (inputbox->widget.widget_class == &txt_inputbox_class)
    {
        char **value = (char **) inputbox->value;

        if (*value != NULL)
        {
            TXT_StringCopy(inputbox->buffer, *value, inputbox->size);
        }
        else
        {
            TXT_StringCopy(inputbox->buffer, "", inputbox->buffer_len);
        }
    }
    else if (inputbox->widget.widget_class == &txt_int_inputbox_class)
    {
        int *value = (int *) inputbox->value;
        TXT_snprintf(inputbox->buffer, inputbox->buffer_len, "%i", *value);
    }
}

static void StartEditing(txt_inputbox_t *inputbox)
{
    #ifdef USE_VIRTUALKEYBOARD
    VKBCreateWindow(inputbox, 0);
    #endif // USE_VIRTUALKEYBOARD
    // Integer input boxes start from an empty buffer:

    if (inputbox->widget.widget_class == &txt_int_inputbox_class)
    {
        TXT_StringCopy(inputbox->buffer, "", inputbox->buffer_len);
    }
    else
    {
        SetBufferFromValue(inputbox);
    }

    inputbox->editing = 1;
}

static void FinishEditing(txt_inputbox_t *inputbox)
{
    if (!inputbox->editing)
    {
        return;
    }

    // Save the new value back to the variable.

    if (inputbox->widget.widget_class == &txt_inputbox_class)
    {
        free(*((char **)inputbox->value));
        *((char **) inputbox->value) = strdup(inputbox->buffer);
    }
    else if (inputbox->widget.widget_class == &txt_int_inputbox_class)
    {
        *((int *) inputbox->value) = atoi(inputbox->buffer);
    }

    TXT_EmitSignal(&inputbox->widget, "changed");

    inputbox->editing = 0;
}

static void TXT_InputBoxSizeCalc(TXT_UNCAST_ARG(inputbox))
{
    TXT_CAST_ARG(txt_inputbox_t, inputbox);

    // Enough space for the box + cursor

    inputbox->widget.w = inputbox->size + 1;
    inputbox->widget.h = 1;
}

static void TXT_InputBoxDrawer(TXT_UNCAST_ARG(inputbox))
{
    TXT_CAST_ARG(txt_inputbox_t, inputbox);
    int focused;
    int i;
    int chars;
    int w;

    focused = inputbox->widget.focused;
    w = inputbox->widget.w;

    // Select the background color based on whether we are currently
    // editing, and if not, whether the widget is focused.

    if (inputbox->editing && focused)
    {
        TXT_BGColor(TXT_COLOR_BLACK, 0);
    }
    else
    {
        TXT_SetWidgetBG(inputbox);
    }

    if (!inputbox->editing)
    {
        // If not editing, use the current value from inputbox->value.

        SetBufferFromValue(inputbox);
    }

    // If string size exceeds the widget's width, show only the end.

    if (TXT_UTF8_Strlen(inputbox->buffer) > w - 1)
    {
        TXT_DrawString("\xae");
        TXT_DrawUTF8String(
            TXT_UTF8_SkipChars(inputbox->buffer,
                               TXT_UTF8_Strlen(inputbox->buffer) - w + 2));
        chars = w - 1;
    }
    else
    {
        TXT_DrawUTF8String(inputbox->buffer);
        chars = TXT_UTF8_Strlen(inputbox->buffer);
    }

    if (chars < w && inputbox->editing && focused)
    {
        TXT_BGColor(TXT_COLOR_BLACK, 1);
        TXT_DrawString("_");
        ++chars;
    }

    for (i=chars; i < w; ++i)
    {
        TXT_DrawString(" ");
    }
}

static void TXT_InputBoxDestructor(TXT_UNCAST_ARG(inputbox))
{
    TXT_CAST_ARG(txt_inputbox_t, inputbox);

    free(inputbox->buffer);
}

static void Backspace(txt_inputbox_t *inputbox)
{
    unsigned int len;
    char *p;

    len = TXT_UTF8_Strlen(inputbox->buffer);

    if (len > 0)
    {
        p = TXT_UTF8_SkipChars(inputbox->buffer, len - 1);
        *p = '\0';
    }
}

static void AddCharacter(txt_inputbox_t *inputbox, int key)
{
    char *end, *p;

    if (TXT_UTF8_Strlen(inputbox->buffer) < inputbox->size)
    {
        // Add character to the buffer

        end = inputbox->buffer + strlen(inputbox->buffer);
        p = TXT_EncodeUTF8(end, key);
        *p = '\0';
    }
}

static int TXT_InputBoxKeyPress(TXT_UNCAST_ARG(inputbox), int key)
{
    TXT_CAST_ARG(txt_inputbox_t, inputbox);
    unsigned int c;

    if (!inputbox->editing)
    {
        if (key == KEY_ABUTTON)
        {
            StartEditing(inputbox);
            return 1;
        }

        // Backspace or delete erases the contents of the box.

        if ((key == KEY_DEL || key == KEY_BACKSPACE)
         && inputbox->widget.widget_class == &txt_inputbox_class)
        {
            free(*((char **)inputbox->value));
            *((char **) inputbox->value) = strdup("");
        }

        return 0;
    }

    if (key == KEY_ABUTTON)
    {
        FinishEditing(inputbox);
    }

    if (key == KEY_BBUTTON)
    {
        inputbox->editing = 0;
    }

    if (key == KEY_BACKSPACE)
    {
        Backspace(inputbox);
    }

    c = TXT_KEY_TO_UNICODE(key);

    // Add character to the buffer, but only if it's a printable character
    // that we can represent on the screen.
    if (isprint(c)
     || (c >= 128 && TXT_CanDrawCharacter(c)))
    {
        AddCharacter(inputbox, c);
    }

    return 1;
}

static void TXT_InputBoxMousePress(TXT_UNCAST_ARG(inputbox),
                                   int x, int y, int b)
{
    TXT_CAST_ARG(txt_inputbox_t, inputbox);

    if (b == TXT_MOUSE_LEFT)
    {
        // Make mouse clicks start editing the box

        if (!inputbox->editing)
        {
            // Send a simulated keypress to start editing

            TXT_WidgetKeyPress(inputbox, KEY_ABUTTON);
        }
    }
}

static void TXT_InputBoxFocused(TXT_UNCAST_ARG(inputbox), int focused)
{
    TXT_CAST_ARG(txt_inputbox_t, inputbox);

    // Stop editing when we lose focus.

    if (inputbox->editing && !focused)
    {
        FinishEditing(inputbox);
    }
}

txt_widget_class_t txt_inputbox_class =
{
    TXT_AlwaysSelectable,
    TXT_InputBoxSizeCalc,
    TXT_InputBoxDrawer,
    TXT_InputBoxKeyPress,
    TXT_InputBoxDestructor,
    TXT_InputBoxMousePress,
    NULL,
    TXT_InputBoxFocused,
};

txt_widget_class_t txt_int_inputbox_class =
{
    TXT_AlwaysSelectable,
    TXT_InputBoxSizeCalc,
    TXT_InputBoxDrawer,
    TXT_InputBoxKeyPress,
    TXT_InputBoxDestructor,
    TXT_InputBoxMousePress,
    NULL,
    TXT_InputBoxFocused,
};

static txt_inputbox_t *NewInputBox(txt_widget_class_t *widget_class,
                                   void *value, int size)
{
    txt_inputbox_t *inputbox;

    inputbox = malloc(sizeof(txt_inputbox_t));

    TXT_InitWidget(inputbox, widget_class);
    inputbox->value = value;
    inputbox->size = size;
    // 'size' is the maximum number of characters that can be entered,
    // but for a UTF-8 string, each character can take up to four
    // characters.
    inputbox->buffer_len = size * 4 + 1;
    inputbox->buffer = malloc(inputbox->buffer_len);
    inputbox->editing = 0;

    return inputbox;
}

txt_inputbox_t *TXT_NewInputBox(char **value, int size)
{
    return NewInputBox(&txt_inputbox_class, value, size);
}

txt_inputbox_t *TXT_NewIntInputBox(int *value, int size)
{
    return NewInputBox(&txt_int_inputbox_class, value, size);
}

