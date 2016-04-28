#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

//http://seriss.com/people/erco/fltk/#DisableSymbols
// Demonstrate a resizer class for widgets in an Fl_Scroll
// erco 1.0 ??/??/04 - original test program
// erco 1.1 04/21/13 - modernized
//

// CLASS FOR A 'RESIZER BAR' BETWEEN WIDGETS IN AN Fl_Scroll
//
//    > Shows a resize cursor when hovered over
//    > Assumes: * Parent is an Fl_Scroll
//               * All children of Fl_Scroll are vertically arranged
//               * The widget above us has a bottom edge touching our top edge
//                 ie. (w->y()+w->h() == this->y())
//
//   > When this widget is dragged:
//              * The widget above us (with a common edge) will be /resized/ vertically
//              * All children below us will be /moved/ vertically
//
class ResizerBar : public Fl_Box {
    int orig_h;
    int last_y;
    int min_h;                                                        // min height for widget above us
    void HandleDrag(int diff) {
        Fl_Scroll *grp = (Fl_Scroll*)parent();
        int top = y();
        int bot = y()+h();
        // First pass: find widget directly above us with common edge
        //    Possibly clamp 'diff' if widget would get too small..
        //
        for ( int t=0; t<grp->children(); t++ ) {
            Fl_Widget *w = grp->child(t);
            if ( (w->y()+w->h()) == top ) {                           // found widget directly above?
                if ( (w->h()+diff) < min_h ) diff = w->h() - min_h;   // clamp
                w->resize(w->x(), w->y(), w->w(), w->h()+diff);       // change height
                break;                                                // done with first pass
            }
        }
        // Second pass: find widgets below us, move based on clamped diff
        for ( int t=0; t<grp->children(); t++ ) {
            Fl_Widget *w = grp->child(t);
            if ( w->y() >= bot )                                     // found widget below us?
                w->resize(w->x(), w->y()+diff, w->w(), w->h());      // change position
        }
        // Change our position last
        resize(x(),y()+diff,w(),h());
        grp->init_sizes();
        grp->redraw();
    }
public:
    ResizerBar(int X,int Y,int W,int H) : Fl_Box(X,Y,W,H,"///////") {
        orig_h = H;
        last_y = 0;
        min_h = 10;
        align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
        labelfont(FL_COURIER);
        labelsize(H);
        visible_focus(0);
        box(FL_FLAT_BOX);
    }
    void SetMinHeight(int val) { min_h = val; }
    int  GetMinHeight() const { return min_h; }
    int handle(int e) {
        int ret = 0;
        int this_y = Fl::event_y_root();
        switch (e) {
            case FL_FOCUS: ret = 1; break;
            case FL_ENTER: ret = 1; fl_cursor(FL_CURSOR_NS);      break;
            case FL_LEAVE: ret = 1; fl_cursor(FL_CURSOR_DEFAULT); break;
            case FL_PUSH:  ret = 1; last_y = this_y;              break;
            case FL_DRAG:
                HandleDrag(this_y-last_y);
                last_y = this_y;
                ret = 1;
                break;
            default: break;
        }
        return(Fl_Box::handle(e) | ret);
    }
    void resize(int X,int Y,int W,int H) {
        Fl_Box::resize(X,Y,W,orig_h);                                // height of resizer stays constant size
    }
};

// Demonstrate the ResizerBar class above
//     Creates 10 Fl_Multiline_Output widgets with a resizer below each.
//     User can drag the resizers around to change the size of the widget above it.
//     The other widgets below will be moved around inside the scroller to accommodate.
//
#include <FL/Fl_Multiline_Output.H>
int main(int argc, char** argv) {
    Fl_Window win(220,400);
    win.begin();
    Fl_Scroll scr(0,0,220,400);
    scr.begin();
    int resize_h = 5;
    // Create 10 boxes with resizer between each
    for ( int i=0; i<10; i++ ) {
        // Create a multiline output
        Fl_Multiline_Output *o = new Fl_Multiline_Output(0,i*(40+resize_h),200,40);
        o->value("Line one\nLine two\nLine three");
        // And put a Resizer directly below it with a common edge. (The common edge is important)
        new ResizerBar(0,o->y()+o->h(),200,resize_h);
    }
    scr.end();
    win.end();
    win.resizable(scr);
    win.show();
    return(Fl::run());
}


