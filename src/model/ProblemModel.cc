#include "model/ProblemModel.h"
#include "db/db.h"

#include <iostream>

using namespace std;

/*
CREATE TABLE problem(
pid INT(11) PRIMARY KEY AUTO_INCREMENT,
title VARCHAR(200) NOT NULL DEFAULT '',
description TEXT,
apps TEXT,
input TEXT,
output TEXT,
sample_input TEXT,
sample_output TEXT,
submit INT(11) DEFAULT 0,
solved INT(11) DEFAULT 0
);
*/

void ProblemModel::query(vector<Problem> &problems)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from problem");

    MySql mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = nullptr;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                Problem problem(
                    atoi(row[0]),
                    row[1],
                    row[2],
                    row[3],
                    atoi(row[4]),
                    atoi(row[5])
                );
                problems.push_back(problem);
            }
        }
    }
}

Problem ProblemModel::query(int pid)
{
    char sql[1024] = {0};
    sprintf(sql, "select * from problem where pid = %d", pid);

    MySql mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row = mysql_fetch_row(res);
            if(row != nullptr)
            {
                return Problem(
                    atoi(row[0]),
                    row[1],
                    row[2],
                    row[3],
                    atoi(row[4]),
                    atoi(row[5])
                );
            }
        }
    }
}

void ProblemModel::updateSubmit(Problem& problem)
{
    char sql[1024] = {0};
    sprintf(sql,"update problem set submit = %d where pid = %d", problem.getSubmit(), problem.getPid());

    MySql mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            return;
        }
    }
    return;
}

void ProblemModel::updateSolved(Problem& problem)
{
    char sql[1024] = {0};
    sprintf(sql,"update problem set solved = %d where pid = %d", problem.getSolved(), problem.getPid());

    MySql mysql;
    if(mysql.connect())
    {
        if(mysql.update(sql))
        {
            return;
        }
    }
    return;
}