#ifndef MATRIXSTACK_H
#define MATRIXSTACK_H

#include "Base.h"
#include "MathInc.h"

enum MATRIX_STACK_TYPE {
    MATRIX_STACK_MODELVIEW,
    MATRIX_STACK_PROJECTION,
};

class MatrixStack {
public:
    MatrixStack() {
        initMatrixStack();
    }
    virtual ~MatrixStack() {}

    void pushMatrix(MATRIX_STACK_TYPE type);
    void popMatrix(MATRIX_STACK_TYPE type);
    void loadIdentityMatrix(MATRIX_STACK_TYPE type);
    void loadMatrix(MATRIX_STACK_TYPE type, const Matrix& mat);
    void multiplyMatrix(MATRIX_STACK_TYPE type, const Matrix& mat);
    Matrix getMatrix(MATRIX_STACK_TYPE type);
    void resetMatrixStack() {
        initMatrixStack();
    }

protected:
    void initMatrixStack();

    std::stack<Matrix> m_stackModelView;
    std::stack<Matrix> m_stackProj;
};


#endif // MATRIXSTACK_H
