#ifndef CBFVIEW_H
#define CBFVIEW_H

#include "cboolfunction.h"

// The CBFView class

static const TStr TableBorderStyle = ("cellspacing = '3' "
                           "cellpadding = '5' "
                           "border = '0' "
                           "style = 'border-style:solid;'");

static const TStr BgColorC = ("bgcolor = '#cccccc'");
static const TStr BgColorE = ("bgcolor = '#eeeeee'");
static const TStr BgColorF = ("bgcolor = '#ffffff'");

class CBFView
{
public:

    CBFView() {}
    explicit CBFView(CBoolFunction *f,const TStr &name,const TStr &path = "");

    CBFView(CBFView *bf);
    virtual ~CBFView();

    TStr view() const;
    TStr viewDIMACS() const;
    TStr viewDis() const;
    TStr viewDisSort() const;
    TStr viewVar() const;
    TStr viewVarSort() const;
    TStr viewSat() const;

    static TStr suffix() {return "cnf";}

    const CBoolFunction::TSPtr& boolFunction() const {return m_bf;}
    const TStr& boolFunctionName() const {return m_name;}
    const TStr& pathFile() const {return m_pathFile;}
    TStr  absoluteFileName() const {return TStr("%1/%2.%3")
                                                .arg(m_pathFile)
                                                .arg(m_name)
                                                .arg(suffix());}

    TStr  baseFileName() const {return TStr("%1.%2").arg(m_name).arg(suffix());}

    void setBoolFunction(const CBoolFunction::TSPtr &bf) {m_bf = bf;}
    void setBoolFunctionName(const TStr &name) {m_name = name;}
    void setPathFile(const TStr &path) {m_pathFile = path;}

    inline TStr htmlVar(int var) const;
    inline TStr htmlDis(int dis) const;

    static const TStr descriptView;
    static const TStr descriptViewDIMACS;
    static const TStr descriptViewDis;
    static const TStr descriptViewDisSort;
    static const TStr descriptViewVar;
    static const TStr descriptViewVarSort;

protected:

    TVLst numericSort(const TLLst & lst,int len = 0) const;

private:

    CBoolFunction::TSPtr m_bf;
    TStr m_name;
    TStr m_pathFile;

};

Q_DECLARE_METATYPE(CBFView)


#endif // CBFVIEW_H
