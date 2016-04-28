#include "FL/Fl.H"

#include "FL/Fl_Double_Window.H"

#include "FL/Fl_Group.H"

#include "FL/Fl_Button.H"

#include "FL/Fl_Input.H"

#include "FL/Fl_Multiline_Input.H"

#include "FL/Fl_Box.H"

//http://blog.163.com/dingmaotu@126/blog/static/214843020119183625748/
int main(int argc, char **argv)
{

      Fl_Window *w = new Fl_Window(100, 200, 460, 320, "Fltk布局");

      //size_range只可以用在顶层窗口，设置窗口resize的范围。它有7个参数，但只有前4个

      //是在各个系统下都有效的。前两个是最小宽度和最小高度。后两个是最大宽度和

      //最大高度。后两个设置成零，说明无限制。如果设置成和前面最小值一样，则不可变动

      //大小。例如

      //w->size_range(w->w(), w->h(), 0, w->h());

      //设置窗口高度不变，宽度可变。

      w->size_range(w->w(), w->h(), 0, 0);

     

      Fl_Group * group1 = new Fl_Group(10, 10, w->w()-20, 30);

      Fl_Input *input1 = new Fl_Input(50, 10, w->w()-175, 30, "姓名:");

      Fl_Button *b1 = new Fl_Button(w->w()-110, 10, 100, 30, "检查!");

      group1->end();

      group1->resizable(input1);

     

      Fl_Group * group2 = new Fl_Group(10, 50, w->w()-20, 30);

      Fl_Input *input2 = new Fl_Input(50, 50, w->w()-175, 30, "邮件:");

      Fl_Button *b2 = new Fl_Button(w->w()-110, 50, 100, 30, "检查!");

      group2->end();

      group2->resizable(input2);

      

      Fl_Multiline_Input * comments = new Fl_Multiline_Input(50, 100, w->w()-60, w->h()-150, "评论:");

      

      Fl_Group *group3 = new Fl_Group(10, w->h()-10-30, w->w()-20, 30);

      Fl_Box *b  = new Fl_Box(10, w->h()-10-30, group3->w() - 100, 30);  //Fl_Box是默认不可见的  

      Fl_Button *b3 = new Fl_Button(w->w()-10-100, w->h()-10-30, 100, 30, "好的");

      group3->end();

      group3->resizable(b);

      

      w->end();      

      w->resizable(comments);

      w->show();

      return Fl::run();

}  
