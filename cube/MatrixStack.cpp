#include "MatrixStack.h"

void MatrixStack::pushMatrix(MATRIX_STACK_TYPE type) {

    switch (type) {
    case MATRIX_STACK_MODELVIEW:
        m_stackModelView.push(m_stackModelView.top());
        break;
    case MATRIX_STACK_PROJECTION:
        m_stackProj.push(m_stackProj.top());
        break;
    default:
        ASSERT(false, "unknow matrix stack type");
        break;
    }
}

void MatrixStack::popMatrix(MATRIX_STACK_TYPE type) {
    switch (type) {
    case MATRIX_STACK_MODELVIEW:
        m_stackModelView.pop();
        break;
    case MATRIX_STACK_PROJECTION:
        m_stackProj.pop();
        break;
    default:
        ASSERT(false, "unknow matrix stack type");
        break;
    }
}

void MatrixStack::loadIdentityMatrix(MATRIX_STACK_TYPE type) {
    switch (type) {
    case MATRIX_STACK_MODELVIEW:
        m_stackModelView.top() = Matrix::identity();
        break;
    case MATRIX_STACK_PROJECTION:
        m_stackProj.top() = Matrix::identity();
        break;
    default:
        ASSERT(false, "unknow matrix stack type");
        break;
    }
}

void MatrixStack::loadMatrix(MATRIX_STACK_TYPE type, const Matrix& mat) {

    switch (type) {
    case MATRIX_STACK_MODELVIEW:
        m_stackModelView.top() = mat;
        break;
    case MATRIX_STACK_PROJECTION:
        m_stackProj.top() = mat;
        break;
    default:
        ASSERT(false, "unknow matrix stack type");
        break;
    }
}

void MatrixStack::multiplyMatrix(MATRIX_STACK_TYPE type, const Matrix& mat) {
    switch (type) {
    case MATRIX_STACK_MODELVIEW:
        m_stackModelView.top() *= mat;
        break;
    case MATRIX_STACK_PROJECTION:
        m_stackProj.top() *= mat;
        break;
    default:
        ASSERT(false, "unknow matrix stack type");
        break;
    }
}

Matrix MatrixStack::getMatrix(MATRIX_STACK_TYPE type) {
    Matrix mat;
    switch (type) {
    case MATRIX_STACK_MODELVIEW:
        mat = m_stackModelView.top();
        break;
    case MATRIX_STACK_PROJECTION:
        mat = m_stackProj.top();
        break;
    default:
        ASSERT(false, "unknow matrix stack type, will return modelview matrix instead");
        mat = m_stackModelView.top();
        break;
    }
    return mat;
}


void MatrixStack::initMatrixStack() {
    while (!m_stackModelView.empty()) {
        m_stackModelView.pop();
    }

    while (!m_stackProj.empty()) {
        m_stackProj.pop();
    }

    m_stackModelView.push(Matrix::identity());
    m_stackProj.push(Matrix::identity());
}
