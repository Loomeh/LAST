#include "components.h"

typedef struct _LSPrevSegment {
    LSComponent base;
    GtkWidget* container;
    GtkWidget* previous_segment_label;
    GtkWidget* previous_segment;
} LSPrevSegment;
extern LSComponentOps ls_prev_segment_operations;

#define PREVIOUS_SEGMENT "Previous segment"
#define LIVE_SEGMENT "Live segment"

LSComponent* ls_component_prev_segment_new()
{
    LSPrevSegment* self;

    self = malloc(sizeof(LSPrevSegment));
    if (!self) {
        return NULL;
    }
    self->base.ops = &ls_prev_segment_operations;

    self->container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    add_class(self->container, "footer");
    gtk_widget_show(self->container);

    self->previous_segment_label = gtk_label_new(PREVIOUS_SEGMENT);
    add_class(self->previous_segment_label, "prev-segment-label");
    gtk_widget_set_halign(self->previous_segment_label,
        GTK_ALIGN_START);
    gtk_widget_set_hexpand(self->previous_segment_label, TRUE);
    gtk_container_add(GTK_CONTAINER(self->container),
        self->previous_segment_label);
    gtk_widget_show(self->previous_segment_label);

    self->previous_segment = gtk_label_new(NULL);
    add_class(self->previous_segment, "prev-segment");
    gtk_widget_set_halign(self->previous_segment, GTK_ALIGN_END);
    gtk_container_add(GTK_CONTAINER(self->container), self->previous_segment);
    gtk_widget_show(self->previous_segment);

    return (LSComponent*)self;
}

static void prev_segment_delete(LSComponent* self)
{
    free(self);
}

static GtkWidget* prev_segment_widget(LSComponent* self)
{
    return ((LSPrevSegment*)self)->container;
}

static void prev_segment_show_game(LSComponent* self_,
    ls_game* game, ls_timer* timer)
{
    LSPrevSegment* self = (LSPrevSegment*)self_;
    remove_class(self->previous_segment, "behind");
    remove_class(self->previous_segment, "losing");
    remove_class(self->previous_segment, "best-segment");
}

static void prev_segment_clear_game(LSComponent* self_)
{
    LSPrevSegment* self = (LSPrevSegment*)self_;
    gtk_label_set_text(GTK_LABEL(self->previous_segment_label),
        PREVIOUS_SEGMENT);
    gtk_label_set_text(GTK_LABEL(self->previous_segment), "");
}

static void prev_segment_draw(LSComponent* self_, ls_game* game,
    ls_timer* timer)
{
    LSPrevSegment* self = (LSPrevSegment*)self_;
    const char* label;
    char str[256];
    int prev, curr = timer->curr_split;
    if (curr == game->split_count) {
        --curr;
    }

    remove_class(self->previous_segment, "best-segment");
    remove_class(self->previous_segment, "behind");
    remove_class(self->previous_segment, "losing");
    remove_class(self->previous_segment, "delta");
    gtk_label_set_text(GTK_LABEL(self->previous_segment), "-");

    label = PREVIOUS_SEGMENT;
    if (timer->segment_deltas[curr] > 0) {
        // Live segment
        label = LIVE_SEGMENT;
        remove_class(self->previous_segment, "best-segment");
        add_class(self->previous_segment, "behind");
        add_class(self->previous_segment, "losing");
        add_class(self->previous_segment, "delta");
        ls_delta_string(str, timer->segment_deltas[curr]);
        gtk_label_set_text(GTK_LABEL(self->previous_segment), str);
    } else if (curr) {
        prev = timer->curr_split - 1;
        // Previous segment
        if (timer->curr_split) {
            prev = timer->curr_split - 1;
            if (timer->segment_deltas[prev]) {
                if (timer->split_info[prev]
                    & LS_INFO_BEST_SEGMENT) {
                    add_class(self->previous_segment, "best-segment");
                } else if (timer->segment_deltas[prev] > 0) {
                    add_class(self->previous_segment, "behind");
                    add_class(self->previous_segment, "losing");
                }
                add_class(self->previous_segment, "delta");
                ls_delta_string(str, timer->segment_deltas[prev]);
                gtk_label_set_text(GTK_LABEL(self->previous_segment), str);
            }
        }
    }
    gtk_label_set_text(GTK_LABEL(self->previous_segment_label), label);
}

LSComponentOps ls_prev_segment_operations = {
    .delete = prev_segment_delete,
    .widget = prev_segment_widget,
    .show_game = prev_segment_show_game,
    .clear_game = prev_segment_clear_game,
    .draw = prev_segment_draw
};
