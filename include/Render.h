#pragma once

#include "ctemplate/template.h"
#include <string>

using namespace std;

const string TEMPLATEBASE = "../template/ctemplate/";

namespace Render
{
    void RenderUser(string& html)
    {
        ctemplate::TemplateDictionary dict("user");
        dict.SetValue("username","admin");
        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "user.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html,&dict);
    }
}