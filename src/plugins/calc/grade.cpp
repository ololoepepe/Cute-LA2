#include "grade.h"

#include <QString>

Grade::GradeType Grade::gradeFromString(const QString &s)
{
    static const Qt::CaseSensitivity Cs = Qt::CaseInsensitive;
    if (!s.compare("D", Cs))
        return DGrade;
    else if (!s.compare("C", Cs))
        return CGrade;
    else if (!s.compare("B", Cs))
        return BGrade;
    else if (!s.compare("A", Cs))
        return AGrade;
    else if (!s.compare("S", Cs))
        return SGrade;
    else
        return NoGrade;
}

QString Grade::gradeToString(GradeType grade)
{
    switch (grade) {
    case NoGrade:
        return "No";
    case DGrade:
        return "D";
    case CGrade:
        return "C";
    case BGrade:
        return "B";
    case AGrade:
        return "A";
    case SGrade:
        return "S";
    default:
        return "";
    }
}
