/*
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
    Editor Field;
    String File;
    float DistMaxX;
    float DistMaxY;
    char Side;
    char ShowLines;
    char IntellisenseOpen;
} EDisplay;

EDisplay EDisplay_New(float x,float y,float w,float h,int Lines,char *Path, int Columns, int Rows, int CharSizeX, int CharSizeY, int AlxFontSizeX, int AlxFontSizeY,char* Syntax,char* File){
    EDisplay d;
    d.Field = Editor_New(Input_New(INPUT_MAXLENGTH,Lines),(Rect){ { x,y },{ w,h } },Path,Columns,Rows,CharSizeX,CharSizeY,AlxFontSizeX,AlxFontSizeY,BLACK);
    //Input_SetText(&d.Field.In,"");
    Editor_SetSyntax(&d.Field,Syntax);
    d.File = String_Make(File);

    d.DistMaxX = 5.0f;
    d.DistMaxY = 5.0f;

    d.Side = EDISPLAY_SIDE_NONE;
    d.ShowLines = 0;
    d.IntellisenseOpen = 0; 
    return d;
}
void EDisplay_Update(EDisplay* e,States* strokes,Vec2 Mouse){
    Editor_Update(&e->Field,strokes,GetMouse());
    
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
        
        if(Input_Stroke(&e->Field.In,ALX_KEY_CTRL).DOWN && Input_Stroke(&e->Field.In,ALX_KEY_SHIFT).PRESSED){
            e->IntellisenseOpen = !e->IntellisenseOpen;
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
    Editor_Render(Target,Target_Width,Target_Height,&e->Field);

    if(e->IntellisenseOpen){
        int CurserX = Input_CurserX(&e->Field.In,e->Field.In.Curser);
        int CurserY = Input_CurserY(&e->Field.In,e->Field.In.Curser);
        int posx = e->Field.r.p.x+(CurserX + e->Field.ScrollX) *  e->Field.AlxFont.CharSizeX - e->Field.AlxFont.CharSizeX;
        int posy = e->Field.r.p.y+(CurserY + e->Field.ScrollY) * (e->Field.AlxFont.CharSizeY * INPUT_GAP_FAKTOR) + e->Field.AlxFont.CharSizeY;
        int boxw = 100;
        int boxh = 100;
        
        posx = F32_Clamp(posx,e->Field.r.p.x,e->Field.r.p.x + e->Field.r.d.x - boxw);
        posx = F32_Clamp(posx,e->Field.r.p.x,e->Field.r.p.y + e->Field.r.d.y - boxh);
        Rect_RenderXX(Target,Target_Width,Target_Height,posx,posy,boxw,boxh,BLACK);
        Rect_RenderXXWire(Target,Target_Width,Target_Height,posx,posy,boxw,boxh,WHITE,1.0f);
    }
}
void EDisplay_Free(EDisplay* d){
    Editor_Free(&d->Field);
}

*/

#include "/home/codeleaded/System/Static/Container/CVector.h"

#include "/home/codeleaded/System/Static/Library/ComponentPack.h"
#include "/home/codeleaded/System/Static/Library/ComponentML.h"
#include "/home/codeleaded/System/Static/Library/ComponentDefines.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/FileChooser.h"

#define FILE_WIDTH      150

Scene scene;
ComponentPack cg;
ComponentML cml;
CStr fileinBuffer;
CVector filesOpen;

Editor* IDE_GetTB(){
    Node* selected = scene.First->Next->Next;
    return (Editor*)selected->Memory;
}

void Button_File_EventHandler(void* parent,Button* b,ButtonEvent* e){
    if(e->ButtonId == ALX_MOUSE_L && e->eid == EVENT_PRESSED){
        const int index = (int)((b->renderable.rect.p.x + 1.0f - 100.0f) / FILE_WIDTH);
        CStr path = *(CStr*)CVector_Get(&filesOpen,index);
        
        Editor* tb = IDE_GetTB();
        if(tb){
            CStr content = Files_ReadT(path);
            if(content){
                Input_SetText(&tb->In,content);
                CStr_Set(&fileinBuffer,path);
                CStr_Free(&content);
            }else{
                printf("[IDE]: File -> %s(%d) not found!\n",path,index);
            }
        }
    }
}
void Button_Open_EventHandler(Button* b,Component* c,ButtonEvent* e){
    if(e->ButtonId == ALX_MOUSE_L && e->eid == EVENT_PRESSED){
        FileChooser fc = FileChooser_New("","/home/codeleaded/Hecke/C/Gui_IDE_New/code",(NameTypePair[]){
            NameTypePair_New("All Files","*.*"),
            NameTypePair_Null()
        });
        CStr_Set(&fileinBuffer,fc);
        CVector_Push(&filesOpen,(CStr[]){ CStr_Cpy(fileinBuffer) });

        CVector_Print(&filesOpen);

        printf("Choosen: '%s'\n",fileinBuffer);

        CStr name = Files_NameFull(fileinBuffer);
        const int count = scene.size - 3;
        Scene_Add(&scene,(Button[]){ Button_New(
            NULL,
            name,
            (void(*)(void*,Label*,LabelEvent*))Button_File_EventHandler,
            AlxFont_MAKE_HIGH(8,16),
            (Vec2){ 8,16 },
            (Rect){ 100 + count * FILE_WIDTH,0.0f,FILE_WIDTH,20.0f },
            ALIGN_HORI_CENTER | ALIGN_VERT_CENTER,
            0xFF000044,
            0xFFFFFFFF
            )},sizeof(Button)
        );

        CStr_Free(&name);
        FileChooser_Free(&fc);
    }
}
void Button_Save_EventHandler(Button* b,Component* c,ButtonEvent* e){
    if(e->ButtonId == ALX_MOUSE_L && e->eid == EVENT_PRESSED){
        if(fileinBuffer){
            Editor* tb = IDE_GetTB();
            if(tb) Files_WriteT(fileinBuffer,tb->In.Buffer.Memory,tb->In.Buffer.size);
        }
    }
}

void Std_EventHandler(void* parent,void* r,EventId* e){
    Scene* s = (Scene*)parent;
    Renderable* ra = (Renderable*)r;
    Component* c = Component_Scene_FindRR(&cg,ra);

    if(c){
        if(CStr_Cmp(c->name,"open")) Button_Open_EventHandler((Button*)ra,c,(ButtonEvent*)e);
        if(CStr_Cmp(c->name,"save")) Button_Save_EventHandler((Button*)ra,c,(ButtonEvent*)e);

        //Component_Print(&cg);
        //Scene_Print(&scene);
    }
}

void Setup(AlxWindow* w){
    scene = Scene_New();

	cml = ComponentML_New(
        KeywordMap_Make((KeywordRP[]){
            KEYWORD_END
        }),
        OperatorMap_Make((OperatorRP[]){
            OperatorRP_Make((TT_Type[]){ TOKEN_EQUAL_SIGN,TOKEN_END },                          TOKEN_CUSTOM_ASS,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_PLUS_SIGN,TOKEN_END },                           TOKEN_CUSTOM_ADD,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_MINUS_SIGN,TOKEN_END },                          TOKEN_CUSTOM_SUB,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ASTERISK,TOKEN_END },                            TOKEN_CUSTOM_MUL,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_SLASH,TOKEN_END },                               TOKEN_CUSTOM_DIV,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_MINUS_SIGN,TOKEN_MINUS_SIGN,TOKEN_END },         TOKEN_CUSTOM_DEC,ARGS_IGNORE),
            
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_L,TOKEN_END },                    TOKEN_CUSTOM_TAGL,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_R,TOKEN_END },                    TOKEN_CUSTOM_TAGR,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_L,TOKEN_SLASH,TOKEN_END },        TOKEN_CUSTOM_TAGC,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_L,TOKEN_EXCLAMATION_MARK,
                                         TOKEN_END },                                           TOKEN_CUSTOM_TAGLEX0,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_L,TOKEN_EXCLAMATION_MARK,
                                         TOKEN_MINUS_SIGN,TOKEN_END },                          TOKEN_CUSTOM_TAGLEX1,ARGS_IGNORE),
    
            OperatorRP_Make((TT_Type[]){ TOKEN_MINUS_SIGN,TOKEN_MINUS_SIGN,TOKEN_END },         TOKEN_CUSTOM_LINECOMMENT,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_ANGLE_BRACKETS_L,TOKEN_EXCLAMATION_MARK,
                                         TOKEN_MINUS_SIGN,TOKEN_MINUS_SIGN,TOKEN_END },         TOKEN_CUSTOM_BLOCKCOMMENTSTART,ARGS_IGNORE),
            OperatorRP_Make((TT_Type[]){ TOKEN_MINUS_SIGN,TOKEN_MINUS_SIGN,
                                         TOKEN_ANGLE_BRACKETS_R,TOKEN_END },                    TOKEN_CUSTOM_BLOCKCOMMENTEND,ARGS_IGNORE),
            OperatorRP_End()
        }),
        DirectiveMap_Make((Directive[]){
            Directive_Comment_Block(TOKEN_CUSTOM_BLOCKCOMMENTSTART,TOKEN_CUSTOM_BLOCKCOMMENTEND,True),
            Directive_Compress_Tag(TOKEN_CUSTOM_TAGOPEN,TOKEN_CUSTOM_TAGL,TOKEN_CUSTOM_TAGR),
            Directive_Compress_Tag(TOKEN_CUSTOM_TAGCLOSE,TOKEN_CUSTOM_TAGC,TOKEN_CUSTOM_TAGR),
            Directive_End()
        }),
        TT_TypeMap_Make((TT_Type[]){
            TOKEN_NEWLINE,
            TOKEN_END
        }),
        ReseterMap_Make((Reseter[]){ 
            // operators who might have two versions: 1 arg vs. 2 args are not listed: -
            { TOKEN_NONE,RESETER_NONE },
            { TOKEN_NUMBER,RESETER_ADD1 },
            { TOKEN_FLOAT,RESETER_ADD1 },
            { TOKEN_STRING,RESETER_ADD1 },
            
            { TOKEN_SPACE,RESETER_NONE },
            { TOKEN_NEWLINE,RESETER_NONE },// without semicolon like python
            { TOKEN_CARTURN,RESETER_NONE },
            { TOKEN_HTAB,RESETER_NONE },
            { TOKEN_VTAB,RESETER_NONE },
    
            { TOKEN_COMMA,RESETER_RST },

            { TOKEN_PARENTHESES_R,RESETER_TO1 },
            { TOKEN_SQUARE_BRACKETS_R,RESETER_NONE },
            { TOKEN_CURLY_BRACES_R,RESETER_NONE },
            //{ TOKEN_ANGLE_BRACKETS_R,RESETER_NONE }, //also < and > operator
            { TOKEN_DOUBLE_ANGLE_BRACKETS_R,RESETER_NONE },
            { TOKEN_WHITE_SQUARE_BRACKETS_R,RESETER_NONE },
            
            { TOKEN_PARENTHESES_L,RESETER_RST },
            { TOKEN_SQUARE_BRACKETS_L,RESETER_RST },
            { TOKEN_CURLY_BRACES_L,RESETER_RST },
            //{ TOKEN_ANGLE_BRACKETS_L,RESETER_RST }, //also < and > operator
            { TOKEN_DOUBLE_ANGLE_BRACKETS_L,RESETER_RST },
            { TOKEN_WHITE_SQUARE_BRACKETS_L,RESETER_RST },
            { TOKEN_SEMICOLON,RESETER_RST },
            { TOKEN_COLON,RESETER_RST },
            { TOKEN_EXCLAMATION_MARK,RESETER_RST },
            { TOKEN_QUESTION_MARK,RESETER_RST },
            { TOKEN_ELLIPSIS,RESETER_RST },
            { TOKEN_PLUS_SIGN,RESETER_RST },
            { TOKEN_MULTIPLICATION_SIGN,RESETER_RST },
            { TOKEN_DIVISION_SIGN,RESETER_RST },
            { TOKEN_EQUAL_SIGN,RESETER_RST },
            { TOKEN_NOT_EQUAL_SIGN,RESETER_RST },
            { TOKEN_LESS_THAN,RESETER_RST },
            { TOKEN_GREATER_THAN,RESETER_RST },
            { TOKEN_LESS_THAN_OR_EQUAL_TO,RESETER_RST },
            { TOKEN_GREATER_THAN_OR_EQUAL_TO,RESETER_RST },
            { TOKEN_APPROXIMATION_SIGN,RESETER_RST },
            { TOKEN_PLUS_MINUS_SIGN,RESETER_RST },
            { TOKEN_AMPERSAND,RESETER_RST },
            { TOKEN_AT_SIGN,RESETER_RST },
            { TOKEN_ASTERISK,RESETER_RST },
            { TOKEN_CARET,RESETER_RST },
            { TOKEN_TILDE,RESETER_RST },
            { TOKEN_SLASH,RESETER_RST },
            { TOKEN_BACKSLASH,RESETER_RST },
            { TOKEN_UNDERSCORE,RESETER_RST },
            { TOKEN_PIPE,RESETER_RST },
            { TOKEN_PERCENT,RESETER_RST },
            { TOKEN_HASH_POUND_SIGN,RESETER_RST },
            { TOKEN_SINGLE_QUOTE,RESETER_RST },
            { TOKEN_DOUBLE_QUOTE,RESETER_RST },
            { TOKEN_BACKTICK,RESETER_RST },
            { TOKEN_PRIME,RESETER_RST },
            { TOKEN_DOUBLE_PRIME,RESETER_RST },
            { TOKEN_SECTION_SYMBOL,RESETER_RST },
            { TOKEN_DEGREE_SYMBOL,RESETER_RST },
            { TOKEN_FRACTION_SLASH,RESETER_RST },
            { TOKEN_INTERROBANG,RESETER_RST },
            { TOKEN_BULLET_POINT,RESETER_RST },
    
            { TOKEN_CAST,RESETER_ADD1 },
            { TOKEN_FUNCTION,RESETER_ADD1 },
            { TOKEN_CONSTSTRING_SINGLE,RESETER_ADD1 },
            { TOKEN_CONSTSTRING_DOUBLE,RESETER_ADD1 },

            { TOKEN_END,RESETER_RST }
        })
    );
    
    ComponentML_Source(&cml,"./code/Main.alxml");
    //TokenMap_Print(&cml.tm);

    cg = ComponentML_Build(&cml);
    Component_Scene_Push_R(&cg,&scene);
    Component_Scene_Event_R(&cg,&scene,Std_EventHandler);

    fileinBuffer = NULL;
    filesOpen = CVector_New(sizeof(CStr),(void*)CStr_Free,NULL,(void*)CStr_PCmp,(void*)CStr_print);
}
void Update(AlxWindow* w){
    Scene_Update(&scene,window.Strokes,GetMouse(),GetMouseBefore());

	Clear(BLACK);

	Scene_Render(WINDOW_STD_ARGS,&scene);
}
void Delete(AlxWindow* w){
	Scene_Free(&scene);
    Component_Free(&cg);
    ComponentML_Free(&cml);

    CStr_Free(&fileinBuffer);
    CVector_Free(&filesOpen);
}

int main(int argc,char** argv){
    if(Create("IDE - New",1900,1000,1,1,Setup,Update,Delete))
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
    Editor Field;
} EDisplay;

EDisplay EDisplay_New(float x,float y,float w,float h,int Lines,int FontSize,char* Syntax,char* File){
    EDisplay d;
    d.ShowLines = FALSE;
    d.IntellisenseOpen = FALSE;
    d.File = String_Make(File);
    d.Field = Editor_New(Input_New(INPUT_MAXLENGTH,Lines),(Rect){ { x,y },{ w,h } },FONT_PATHS_YANIS,FontSize,FontSize,BLACK);
    //Input_SetText(&d.Field.In,"");
    Editor_SetSyntax(&d.Field,Syntax);
    return d;
}
void EDisplay_Update(EDisplay* e,Vec2 Mouse){
    Editor_Update(&e->Field,GetMouse());
}
void EDisplay_Render(unsigned int* Target,int Target_Width,int Target_Height,EDisplay* e){
    Editor_Render(WINDOW_STD_ARGS,&e->Field);
}
void EDisplay_Free(EDisplay* d){
    Editor_Free(&d->Field);
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