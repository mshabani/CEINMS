#ifndef ModelEvaluationOffline_h
#define ModelEvaluationOffline_h

#include <vector>
#include <string>
#include "ModelEvaluationBase.h"


template <typename NMSmodelT>
class ModelEvaluationOffline : public ModelEvaluationBase {

public:
    ModelEvaluationOffline(NMSmodelT& subject);
    ~ModelEvaluationOffline();

    void setSubject(NMSmodelT& subject);
    void operator()();
    
private:
    void initOfflineCurve();

    NMSmodelT& subject_;
    std::vector< std::string > dofNames_;
    std::vector< std::string > dofNamesWithExtTorque_;
    unsigned noDof_;
};

#include "ModelEvaluationOffline.cpp"

#endif