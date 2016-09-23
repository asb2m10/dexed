/*
  LV2 Sampler Example Plugin UI
  Copyright 2011-2016 David Robillard <d@drobilla.net>

  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <stdlib.h>

#include <gtk/gtk.h>

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/forge.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/log/logger.h"
#include "lv2/lv2plug.in/ns/ext/patch/patch.h"
#include "lv2/lv2plug.in/ns/ext/urid/urid.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"
#include "lv2/lv2plug.in/ns/lv2core/lv2_util.h"

#include "./uris.h"

#define SAMPLER_UI_URI "http://lv2plug.in/plugins/eg-sampler#ui"

typedef struct {
	LV2_Atom_Forge forge;
	LV2_URID_Map*  map;
	LV2_Log_Logger logger;
	SamplerURIs    uris;

	LV2UI_Write_Function write;
	LV2UI_Controller     controller;

	GtkWidget* box;
	GtkWidget* button;
	GtkWidget* label;
	GtkWidget* window;  /* For optional show interface. */
} SamplerUI;

static void
on_load_clicked(GtkWidget* widget,
                void*      handle)
{
	SamplerUI* ui = (SamplerUI*)handle;

	/* Create a dialog to select a sample file. */
	GtkWidget* dialog = gtk_file_chooser_dialog_new(
		"Load Sample",
		NULL,
		GTK_FILE_CHOOSER_ACTION_OPEN,
		GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
		NULL);

	/* Run the dialog, and return if it is cancelled. */
	if (gtk_dialog_run(GTK_DIALOG(dialog)) != GTK_RESPONSE_ACCEPT) {
		gtk_widget_destroy(dialog);
		return;
	}

	/* Get the file path from the dialog. */
	char* filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

	/* Got what we need, destroy the dialog. */
	gtk_widget_destroy(dialog);

#define OBJ_BUF_SIZE 1024
	uint8_t obj_buf[OBJ_BUF_SIZE];
	lv2_atom_forge_set_buffer(&ui->forge, obj_buf, OBJ_BUF_SIZE);

	LV2_Atom* msg = (LV2_Atom*)write_set_file(&ui->forge, &ui->uris,
	                                          filename, strlen(filename));

	ui->write(ui->controller, 0, lv2_atom_total_size(msg),
	          ui->uris.atom_eventTransfer,
	          msg);

	g_free(filename);
}

static LV2UI_Handle
instantiate(const LV2UI_Descriptor*   descriptor,
            const char*               plugin_uri,
            const char*               bundle_path,
            LV2UI_Write_Function      write_function,
            LV2UI_Controller          controller,
            LV2UI_Widget*             widget,
            const LV2_Feature* const* features)
{
	SamplerUI* ui = (SamplerUI*)calloc(1, sizeof(SamplerUI));
	if (!ui) {
		return NULL;
	}

	ui->write      = write_function;
	ui->controller = controller;
	*widget        = NULL;

	// Get host features
	const char* missing = lv2_features_query(
		features,
		LV2_LOG__log,  &ui->logger.log, false,
		LV2_URID__map, &ui->map,        true,
		NULL);
	lv2_log_logger_set_map(&ui->logger, ui->map);
	if (missing) {
		lv2_log_error(&ui->logger, "Missing feature <%s>\n", missing);
		free(ui);
		return NULL;
	}

	// Map URIs and initialise forge
	map_sampler_uris(ui->map, &ui->uris);
	lv2_atom_forge_init(&ui->forge, ui->map);

	// Construct Gtk UI
	ui->box = gtk_vbox_new(FALSE, 4);
	ui->label = gtk_label_new("?");
	ui->button = gtk_button_new_with_label("Load Sample");
	gtk_box_pack_start(GTK_BOX(ui->box), ui->label, TRUE, TRUE, 4);
	gtk_box_pack_start(GTK_BOX(ui->box), ui->button, FALSE, FALSE, 4);
	g_signal_connect(ui->button, "clicked",
	                 G_CALLBACK(on_load_clicked),
	                 ui);

	// Request state (filename) from plugin
	uint8_t get_buf[512];
	lv2_atom_forge_set_buffer(&ui->forge, get_buf, sizeof(get_buf));

	LV2_Atom_Forge_Frame frame;
	LV2_Atom*            msg = (LV2_Atom*)lv2_atom_forge_object(
		&ui->forge, &frame, 0, ui->uris.patch_Get);
	lv2_atom_forge_pop(&ui->forge, &frame);

	ui->write(ui->controller, 0, lv2_atom_total_size(msg),
	          ui->uris.atom_eventTransfer,
	          msg);

	*widget = ui->box;

	return ui;
}

static void
cleanup(LV2UI_Handle handle)
{
	SamplerUI* ui = (SamplerUI*)handle;
	gtk_widget_destroy(ui->button);
	free(ui);
}

static void
port_event(LV2UI_Handle handle,
           uint32_t     port_index,
           uint32_t     buffer_size,
           uint32_t     format,
           const void*  buffer)
{
	SamplerUI* ui = (SamplerUI*)handle;
	if (format == ui->uris.atom_eventTransfer) {
		const LV2_Atom* atom = (const LV2_Atom*)buffer;
		if (lv2_atom_forge_is_object_type(&ui->forge, atom->type)) {
			const LV2_Atom_Object* obj = (const LV2_Atom_Object*)atom;
			const char*            uri = read_set_file(&ui->uris, obj);
			if (uri) {
				gtk_label_set_text(GTK_LABEL(ui->label), uri);
			} else {
				lv2_log_warning(&ui->logger, "Malformed message\n");
			}
		} else {
			lv2_log_error(&ui->logger, "Unknown message type\n");
		}
	} else {
		lv2_log_warning(&ui->logger, "Unknown port event format\n");
	}
}

/* Optional non-embedded UI show interface. */
static int
ui_show(LV2UI_Handle handle)
{
	SamplerUI* ui = (SamplerUI*)handle;

	int argc = 0;
	gtk_init(&argc, NULL);

	ui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_add(GTK_CONTAINER(ui->window), ui->box);
	gtk_widget_show_all(ui->window);
	gtk_window_present(GTK_WINDOW(ui->window));

	return 0;
}

/* Optional non-embedded UI hide interface. */
static int
ui_hide(LV2UI_Handle handle)
{
	return 0;
}

/* Idle interface for optional non-embedded UI. */
static int
ui_idle(LV2UI_Handle handle)
{
	SamplerUI* ui = (SamplerUI*)handle;
	if (ui->window) {
		gtk_main_iteration();
	}
	return 0;
}

static const void*
extension_data(const char* uri)
{
	static const LV2UI_Show_Interface show = { ui_show, ui_hide };
	static const LV2UI_Idle_Interface idle = { ui_idle };
	if (!strcmp(uri, LV2_UI__showInterface)) {
		return &show;
	} else if (!strcmp(uri, LV2_UI__idleInterface)) {
		return &idle;
	}
	return NULL;
}

static const LV2UI_Descriptor descriptor = {
	SAMPLER_UI_URI,
	instantiate,
	cleanup,
	port_event,
	extension_data
};

LV2_SYMBOL_EXPORT
const LV2UI_Descriptor*
lv2ui_descriptor(uint32_t index)
{
	switch (index) {
	case 0:
		return &descriptor;
	default:
		return NULL;
	}
}
