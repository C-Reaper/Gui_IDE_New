#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/ObjectNotation.h"

#define SYNTAX_PATH     "/home/codeleaded/System/SyntaxFiles/Path_Syntax.alxon"
#define SYNTAX_C        "/home/codeleaded/System/SyntaxFiles/C_Syntax.alxon"
#define SYNTAX_OMML     "/home/codeleaded/System/SyntaxFiles/OMML_Syntax.alxon"

#define EDISPLAY_SIDE_NONE          0
#define EDISPLAY_SIDE_NORTH         1
#define EDISPLAY_SIDE_SOUTH         2
#define EDISPLAY_SIDE_WEST          3
#define EDISPLAY_SIDE_EAST          4
#define EDISPLAY_SIDE_NORTHWEST     5
#define EDISPLAY_SIDE_NORTHEAST     6
#define EDISPLAY_SIDE_SOUTHWEST     7
#define EDISPLAY_SIDE_SOUTHEAST     8

typedef struct EDisplay {
    TextBox Field;
    String File;
    float DistMaxX;
    float DistMaxY;
    char Side;
    char ShowLines;
    char IntellisenseOpen;
} EDisplay;

EDisplay EDisplay_New(float x,float y,float w,float h,int Lines,char *Path, int Columns, int Rows, int CharSizeX, int CharSizeY, int AlxFontSizeX, int AlxFontSizeY,char* Syntax,char* File){
    EDisplay d;
    d.Field = TextBox_New(Input_New(INPUT_MAXLENGTH,Lines),(Rect){ { x,y },{ w,h } },Path,Columns,Rows,CharSizeX,CharSizeY,AlxFontSizeX,AlxFontSizeY,BLACK);
    //Input_SetText(&d.Field.In,"");
    TextBox_SetSyntax(&d.Field,Syntax);
    d.File = String_Make(File);

    d.DistMaxX = 5.0f;
    d.DistMaxY = 5.0f;

    d.Side = EDISPLAY_SIDE_NONE;
    d.ShowLines = 0;
    d.IntellisenseOpen = 0; 
    return d;
}
void EDisplay_Update(EDisplay* e,States* strokes,Vec2 Mouse){
    TextBox_Update(&e->Field,strokes,GetMouse());
    
    if(e->Field.In.Enabled){
        if(Input_Stroke(&e->Field.In,ALX_MOUSE_L).PRESSED){
            float we = F32_Abs(Mouse.x - e->Field.r.p.x);
            float ea = F32_Abs(Mouse.x - (e->Field.r.p.x + e->Field.r.d.x));
            float no = F32_Abs(Mouse.y - e->Field.r.p.y);
            float so = F32_Abs(Mouse.y - (e->Field.r.p.y + e->Field.r.d.y));

            if(     no < e->DistMaxY && we < e->DistMaxX)   e->Side = EDISPLAY_SIDE_NORTHWEST;
            else if(no < e->DistMaxY && ea < e->DistMaxX)   e->Side = EDISPLAY_SIDE_NORTHEAST;
            else if(so < e->DistMaxY && we < e->DistMaxX)   e->Side = EDISPLAY_SIDE_SOUTHWEST;
            else if(so < e->DistMaxY && ea < e->DistMaxX)   e->Side = EDISPLAY_SIDE_SOUTHEAST;
            else if(no < e->DistMaxY)                       e->Side = EDISPLAY_SIDE_NORTH;
            else if(so < e->DistMaxY)                       e->Side = EDISPLAY_SIDE_SOUTH;
            else if(we < e->DistMaxX)                       e->Side = EDISPLAY_SIDE_WEST;
            else if(ea < e->DistMaxX)                       e->Side = EDISPLAY_SIDE_EAST;
        }

        if(e->Side!=EDISPLAY_SIDE_NONE){
            if(e->Side == EDISPLAY_SIDE_NORTH){
                e->Field.r.d.y += e->Field.r.p.y - Mouse.y;
                e->Field.r.p.y = Mouse.y;
            }else if(e->Side == EDISPLAY_SIDE_SOUTH){
                e->Field.r.d.y += Mouse.y - (e->Field.r.p.y + e->Field.r.d.y);
            }else if(e->Side == EDISPLAY_SIDE_WEST){
                e->Field.r.d.x += e->Field.r.p.x - Mouse.x;
                e->Field.r.p.x = Mouse.x;
            }else if(e->Side == EDISPLAY_SIDE_EAST){
                e->Field.r.d.x += Mouse.x - (e->Field.r.p.x + e->Field.r.d.x);
            }else if(e->Side == EDISPLAY_SIDE_NORTHWEST){
                e->Field.r.d.y += e->Field.r.p.y - Mouse.y;
                e->Field.r.p.y = Mouse.y;
                e->Field.r.d.x += e->Field.r.p.x - Mouse.x;
                e->Field.r.p.x = Mouse.x;
            }else if(e->Side == EDISPLAY_SIDE_NORTHEAST){
                e->Field.r.d.y += e->Field.r.p.y - Mouse.y;
                e->Field.r.p.y = Mouse.y;
                e->Field.r.d.x += Mouse.x - (e->Field.r.p.x + e->Field.r.d.x);
            }else if(e->Side == EDISPLAY_SIDE_SOUTHWEST){
                e->Field.r.d.y += Mouse.y - (e->Field.r.p.y + e->Field.r.d.y);
                e->Field.r.d.x += e->Field.r.p.x - Mouse.x;
                e->Field.r.p.x = Mouse.x;
            }else if(e->Side == EDISPLAY_SIDE_SOUTHEAST){
                e->Field.r.d.y += Mouse.y - (e->Field.r.p.y + e->Field.r.d.y);
                e->Field.r.d.x += Mouse.x - (e->Field.r.p.x + e->Field.r.d.x);
            }
        }
    }

    if(Input_Stroke(&e->Field.In,ALX_MOUSE_L).RELEASED){
        e->Side = EDISPLAY_SIDE_NONE;
    }

    e->Field.r.d.x = F32_Clamp(e->Field.r.d.x,10.0f,GetWidth());
    e->Field.r.d.y = F32_Clamp(e->Field.r.d.y,10.0f,GetHeight());
    e->Field.r.p.x = F32_Clamp(e->Field.r.p.x,0.0f,GetWidth() - e->Field.r.d.x);
    e->Field.r.p.y = F32_Clamp(e->Field.r.p.y,0.0f,GetHeight() - e->Field.r.d.y);
}
void EDisplay_Render(unsigned int* Target,int Target_Width,int Target_Height,EDisplay* e){
    TextBox_Render(WINDOW_STD_ARGS,&e->Field);
}
void EDisplay_Free(EDisplay* d){
    TextBox_Free(&d->Field);
}


Vector displays;

void Setup(AlxWindow* w){
    displays = Vector_New(sizeof(EDisplay));

    //Vector_Push(&displays,(EDisplay[]){ EDisplay_New(200.0f,  0.0f, 1100.0f, 625.0f,INPUT_MAXLENGTH,16,SYNTAX_C,"./Saved.c") });
    Vector_Push(&displays,(EDisplay[]){ EDisplay_New(0.0f,0.0f,1100.0f,625.0f,INPUT_MAXLENGTH,ALXFONT_PATHS_HIGH,32,64,SYNTAX_C,"./src/Main.c") });
    //Vector_Push(&displays,(EDisplay[]){ EDisplay_New(200.0f,  625.0f,1100.0f, 625.0f,INPUT_MAXLENGTH,16,SYNTAX_C,"./Files/Main.c") });
    //Vector_Push(&displays,(EDisplay[]){ EDisplay_New(1300.0f, 625.0f,1100.0f, 625.0f,INPUT_MAXLENGTH,16,SYNTAX_C,"./Files/File.txt") });

    Setup_ByInit(&displays);
}
void Update(AlxWindow* w){
    for(int i = 0;i<displays.size;i++){
        EDisplay* d = (EDisplay*)Vector_Get(&displays,i);
        EDisplay_Update(d,window.Strokes,GetMouse());
    }

    Clear(BLACK);

    for(int i = 0;i<displays.size;i++){
        EDisplay* d = (EDisplay*)Vector_Get(&displays,i);
        EDisplay_Render(WINDOW_STD_ARGS,d);
    }
}
void Delete(AlxWindow* w){
    Vector_Free(&displays);
}

int main(int argc,char** argv){
    if(Create("IDE",2400,1300,1,1,Setup,Update,Delete))
        Start();
    return 0;
}

/*
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
#include "C:/Wichtig/System/Static/Library/ObjectNotation.h"

#define SYNTAX_PATH     "C:/Wichtig/System/SyntaxFiles/Path_Syntax.alxon"
#define SYNTAX_C        "C:/Wichtig/System/SyntaxFiles/C_Syntax.alxon"

typedef struct EDisplay {
    char ShowLines;
    char IntellisenseOpen;
    String File;
    TextBox Field;
} EDisplay;

EDisplay EDisplay_New(float x,float y,float w,float h,int Lines,int FontSize,char* Syntax,char* File){
    EDisplay d;
    d.ShowLines = FALSE;
    d.IntellisenseOpen = FALSE;
    d.File = String_Make(File);
    d.Field = TextBox_New(Input_New(INPUT_MAXLENGTH,Lines),(Rect){ { x,y },{ w,h } },FONT_PATHS_YANIS,FontSize,FontSize,BLACK);
    //Input_SetText(&d.Field.In,"");
    TextBox_SetSyntax(&d.Field,Syntax);
    return d;
}
void EDisplay_Update(EDisplay* e,Vec2 Mouse){
    TextBox_Update(&e->Field,GetMouse());
}
void EDisplay_Render(unsigned int* Target,int Target_Width,int Target_Height,EDisplay* e){
    TextBox_Render(WINDOW_STD_ARGS,&e->Field);
}
void EDisplay_Free(EDisplay* d){
    TextBox_Free(&d->Field);
}


void list_files(Branch* Parent,const char *path) {
    struct dirent *entry;
    DIR *dp = opendir(path);
    
    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        char* Clipped = CStr_ChopEndFrom(full_path,'/');
        Branch* b = Branch_New((String[]){ String_Make(Clipped) },sizeof(String));
        free(Clipped);

        struct stat statbuf;
        if (stat(full_path, &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {
                Branch_Add(Parent,b);
                printf("Verzeichnis: %s\n", full_path);
                list_files(b,full_path);
            } else if (S_ISREG(statbuf.st_mode)) {
                Branch_Add(Parent,b);
                printf("Datei: %s\n", full_path);
            }
        } else {
            perror("stat");
        }
    }

    closedir(dp);
}

typedef struct Explorer {
    Tree fsystem;
    String ParentDir;
    Rect r;
    Font font;
    Tex t;
    HighLight Syntax;
    int ScrollX;
    int ScrollY;
    unsigned int Fg;
    unsigned int Bg;
} Explorer;

void Explorer_Reload(Explorer* e){
    char* cstr = String_CStr(&e->ParentDir);
    
    list_files(e->fsystem.Root,cstr);
    free(cstr);
}
Explorer Explorer_New(float x,float y,float w,float h,int FontSize,unsigned int Bg,char* ParentDir){
    Explorer d;
    d.fsystem = Tree_New();
    d.ParentDir = String_Make(ParentDir);
    d.r = (Rect){ x,y,w,h };
    d.font = Font_Make(FONT_YANIS,FontSize,FontSize);
    d.t = Vector_New(sizeof(SymbolInfo));
    d.Syntax = Vector_New(sizeof(Pair));
    d.ScrollX = 0;
    d.ScrollY = 0;
    d.Fg = 0xFFFFFFFF;
    d.Bg = Bg;

    HighLight_Set(&d.Syntax,SYNTAX_PATH);
    Explorer_Reload(&d);
    return d;
}

void Explorer_Update(Explorer* e,Vec2 Mouse){
    //if(Input_Stroke(&tb->In,VK_LBUTTON).PRESSED){
    //    if(Mouse.x>=tb->r.p.x && Mouse.x<tb->r.p.x+tb->r.d.x && Mouse.y>=tb->r.p.y && Mouse.y<tb->r.p.y+tb->r.d.y){
    //        tb->In.Enabled = TRUE;
    //    }else{
    //        tb->In.Enabled = FALSE;
    //    }
    //}
    //if(Input_Stroke(&tb->In,VK_LBUTTON).PRESSED){
    //    tb->In.Curser = Input_GetCurser(&tb->In,(Mouse.x - tb->r.p.x) / tb->font.CharSizeX - tb->ScrollX,(Mouse.y - tb->r.p.y) / (tb->font.CharSizeY * INPUT_GAP_FAKTOR) - tb->ScrollY);
    //}
    //if(Input_Stroke(&tb->In,VK_LBUTTON).DOWN){
    //    tb->In.CurserEnd = Input_GetCurser(&tb->In,(Mouse.x - tb->r.p.x) / tb->font.CharSizeX - tb->ScrollX,(Mouse.y - tb->r.p.y) / (tb->font.CharSizeY * INPUT_GAP_FAKTOR) - tb->ScrollY);
    //}
}
void Explorer_RenderFile(unsigned int* Target,int Target_Width,int Target_Height,Explorer* e,Branch* b,int* x,int* y){
    String* s = b->Memory;
    if(s){
        Tex_SyncString(&e->t,s);
        Tex_HighLightString(&e->t,s,&e->Syntax);
        TCStr_RenderSizeFont(Target,Target_Width,Target_Height,&e->font,Vector_Get(&e->t,0),(char*)(s->str.Memory + 0),s->str.size,*x,*y);

        //CStr_RenderSizeFont(Target,Target_Width,Target_Height,&e->font,(unsigned char*)s->str.Memory,s->str.size,*x,*y,WHITE);
    }

    (*y)+=e->font.CharSizeX;
    for(int i = 0;i<b->Childs.size;i++){
        (*x)+=e->font.CharSizeX;
        Explorer_RenderFile(Target,Target_Width,Target_Height,e,*(Branch**)Vector_Get(&b->Childs,i),x,y);
        (*x)-=e->font.CharSizeX;
    }
}
void Explorer_Render(unsigned int* Target,int Target_Width,int Target_Height,Explorer* e){
    Rect_Render(Target,Target_Width,Target_Height,e->r,e->Bg);
    Rect_RenderWire(Target,Target_Width,Target_Height,e->r,e->Fg,1.0f);

    Explorer_RenderFile(Target,Target_Width,Target_Height,e,e->fsystem.Root,(int[]){ e->r.p.x },(int[]){ e->r.p.y });
}

void Explorer_Free(Explorer* e){
    Tree_Free(&e->fsystem);
    String_Free(&e->ParentDir);
    Font_Free(&e->font);

    Vector_Free(&e->t);
    for(int i = 0;i<e->Syntax.size;i++){
        Pair* p = (Pair*)Vector_Get(&e->Syntax,i);
        Pair_Free(p);
    }
    Vector_Free(&e->Syntax);
}


void Setup_ByInit(Vector* EDisplays){
    
    Table t = Table_New();
    Table_Parse(&t,".init.alxon");
    Table_Print(&t);

    int Version = (int)Number_Parse(Table_Access_Entry(&t,"/Version",'/')->Value);
    Branch* Windows = Table_Access_Branch(&t,"/Windows",'/');

    for(int i = 0;i<Windows->Childs.size;i++){
        char Buffer[100];
        
        memset(Buffer,0,100);
        sprintf(Buffer,"/Windows/%d/Path",i);
        char* Path = Table_Access_Entry(&t,Buffer,'/')->Value;
        
        memset(Buffer,0,100);
        sprintf(Buffer,"/Windows/%d/Syntax",i);
        char* Syntax = Table_Access_Entry(&t,Buffer,'/')->Value;
        
        memset(Buffer,0,100);
        sprintf(Buffer,"/Windows/%d/X",i);
        Number X = Number_Parse(Table_Access_Entry(&t,Buffer,'/')->Value);
        
        memset(Buffer,0,100);
        sprintf(Buffer,"/Windows/%d/Y",i);
        Number Y = Number_Parse(Table_Access_Entry(&t,Buffer,'/')->Value);

        memset(Buffer,0,100);
        sprintf(Buffer,"/Windows/%d/W",i);
        Number W = Number_Parse(Table_Access_Entry(&t,Buffer,'/')->Value);

        memset(Buffer,0,100);
        sprintf(Buffer,"/Windows/%d/H",i);
        Number H = Number_Parse(Table_Access_Entry(&t,Buffer,'/')->Value);

        Vector_Push(EDisplays,(EDisplay[]){ EDisplay_New(X,Y,W,H,INPUT_MAXLENGTH,16,Syntax,Path) });
    }
    
    Table_Free(&t);
}


Explorer explorer;
Vector displays;

void Setup(AlxWindow* w){
    explorer = Explorer_New(0.0f,0.0f,200.0f,1200.0f,20,0x00000000,".");
    displays = Vector_New(sizeof(EDisplay));

    //Vector_Push(&displays,(EDisplay[]){ EDisplay_New(200.0f,  0.0f, 1100.0f, 625.0f,INPUT_MAXLENGTH,16,SYNTAX_C,"./Saved.c") });
    //Vector_Push(&displays,(EDisplay[]){ EDisplay_New(1300.0f, 0.0f, 1100.0f, 625.0f,INPUT_MAXLENGTH,16,SYNTAX_C,"./Main.c") });
    //Vector_Push(&displays,(EDisplay[]){ EDisplay_New(200.0f,  625.0f,1100.0f, 625.0f,INPUT_MAXLENGTH,16,SYNTAX_C,"./Files/Main.c") });
    //Vector_Push(&displays,(EDisplay[]){ EDisplay_New(1300.0f, 625.0f,1100.0f, 625.0f,INPUT_MAXLENGTH,16,SYNTAX_C,"./Files/File.txt") });

    Setup_ByInit(&displays);
}

void Update(AlxWindow* w){
    Explorer_Update(&explorer,GetMouse());
    
    for(int i = 0;i<displays.size;i++){
        EDisplay* d = (EDisplay*)Vector_Get(&displays,i);
        EDisplay_Update(d,GetMouse());
    }

    Clear(BLACK);

    Explorer_Render(WINDOW_STD_ARGS,&explorer);

    for(int i = 0;i<displays.size;i++){
        EDisplay* d = (EDisplay*)Vector_Get(&displays,i);
        EDisplay_Render(WINDOW_STD_ARGS,d);
    }
}

void Delete(AlxWindow* w){
    Vector_Free(&displays);
}

int main(int argc,char** argv){
    if(Create("IDE - Alx",2400,1300,1,1,Setup,Update,Delete))
        Start();
    return 0;
}
*/