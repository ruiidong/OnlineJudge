#pragma once

#include "ctemplate/template.h"
#include <string>
#include <vector>

#include "model/Problem.h"
#include "model/Category.h"

using namespace std;

const string TEMPLATEBASE = "../template/ctemplate/";

namespace Render
{
    void RenderUser(string& html, const string& username)
    {
        ctemplate::TemplateDictionary dict("user");
        dict.SetValue("username",username.c_str());
        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "user.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html,&dict);
    }

    void RenderProblem(string& html, const vector<Problem>& problems, const string& username)
    {
        ctemplate::TemplateDictionary dict("all_questions");
        dict.SetValue("username",username.c_str());
        
        for(const auto& problem : problems)
        {
            ctemplate::TemplateDictionary* table_dict = dict.AddSectionDictionary("problem");
            table_dict->SetValue("pid", to_string(problem.getPid()).c_str());
            table_dict->SetValue("title", problem.getTitle().c_str());
            table_dict->SetValue("apps", problem.getApps().c_str());
            table_dict->SetValue("solved", to_string(problem.getSolved()).c_str());
            table_dict->SetValue("submit", to_string(problem.getSubmit()).c_str());
            int odds = problem.getSolved()/problem.getSubmit();
            table_dict->SetValue("odds", to_string(odds).c_str());
        }
        
        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "problemset.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html, &dict);
    }

    void RenderCategory(string& html, vector<Category>& categorys, const string& username)
    {
        ctemplate::TemplateDictionary dict("all_categorys");
        dict.SetValue("username", username.c_str());
        
        for(const auto& category : categorys)
        {
            ctemplate::TemplateDictionary* table_dict = dict.AddSectionDictionary("category");
            table_dict->SetValue("text", category.getText().c_str());
        }

        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "category.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html, &dict);  
    }
}