#pragma once

#include "ctemplate/template.h"
#include <string>
#include <vector>

#include "model/Problem.h"
#include "model/Category.h"
#include "model/User.h"
#include "model/Status.h"

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

    void RenderProblemSet(string& html, const vector<Problem>& problems, const string& username)
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
            double odds = 0;
            if(problem.getSubmit()!=0)
            {
                odds = problem.getSolved()/problem.getSubmit();
            }
            table_dict->SetValue("odds", (to_string(odds) + "%").c_str());
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

    void RenderRanklist(string& html, vector<User>& users, const string& username)
    {
        ctemplate::TemplateDictionary dict("ranklist");
        dict.SetValue("username", username.c_str());

        int i = 1;
        for(const auto& user : users)
        {
            ctemplate::TemplateDictionary* table_dict = dict.AddSectionDictionary("ranklist");
            table_dict->SetValue("rank", to_string(i++).c_str());
            table_dict->SetValue("username", user.getUsername().c_str());
            table_dict->SetValue("solved", to_string(user.getSolved()).c_str());
            table_dict->SetValue("submit", to_string(user.getSubmit()).c_str());
            double odds = 0;
            if(user.getSubmit()!=0)
            {
                odds = user.getSolved()/user.getSubmit();
            }
            table_dict->SetValue("odds", (to_string(odds) + "%").c_str());
        }
        
        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "ranklist.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html, &dict);  
    }

    void RenderStatus(string& html, vector<Status>& statuss, const string& username)
    {
        ctemplate::TemplateDictionary dict("status");
        dict.SetValue("username", username.c_str());

        for(const auto& status : statuss)
        {
            ctemplate::TemplateDictionary* table_dict = dict.AddSectionDictionary("status");
            table_dict->SetValue("sid", to_string(status.getSid()).c_str());
            table_dict->SetValue("username", username);
            table_dict->SetValue("pid", to_string(status.getPid()).c_str());
            table_dict->SetValue("result", status.getResult().c_str());
            table_dict->SetValue("memorylimit", to_string(status.getMemoryLimit()).c_str());
            table_dict->SetValue("timelimit", to_string(status.getTimeLimit()).c_str());
            table_dict->SetValue("language", status.getLanguage().c_str());
            table_dict->SetValue("length", to_string(status.getLength()).c_str());
            table_dict->SetValue("submittime", status.getSubmittime().c_str());
        }
        
        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "status.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html, &dict);  
    }

    void RenderProblem(string& html, const Problem& problem, const string& username)
    {
        ctemplate::TemplateDictionary dict("problem");
        dict.SetValue("username", username.c_str());
        dict.SetValue("title",problem.getTitle().c_str());
        dict.SetValue("description",problem.getDescription().c_str());
        dict.SetValue("pid", to_string(problem.getPid()).c_str());

        ctemplate::Template* tpl;
        tpl = ctemplate::Template::GetTemplate(TEMPLATEBASE + "problem.html", ctemplate::DO_NOT_STRIP);
        tpl->Expand(&html, &dict);  
    }
}