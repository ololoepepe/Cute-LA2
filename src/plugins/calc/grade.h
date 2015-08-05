#ifndef GRADE_H
#define GRADE_H

class QString;

class Grade
{
public:
    enum GradeType
    {
        NoGrade = 0,
        DGrade,
        CGrade,
        BGrade,
        AGrade,
        SGrade
    };
public:
    static GradeType gradeFromString(const QString &s);
    static QString gradeToString(GradeType grade);
};

#endif // GRADE_H
