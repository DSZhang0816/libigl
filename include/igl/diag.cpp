#include "diag.h"

#include "verbose.h"

template <typename T>
IGL_INLINE void igl::diag(
  const Eigen::SparseMatrix<T>& X, 
  Eigen::SparseVector<T>& V)
{
  // Get size of input
  int m = X.rows();
  int n = X.cols();
  V = Eigen::SparseVector<T>((m>n?n:m));
  V.reserve(V.size());

  // Iterate over outside
  for(int k=0; k<X.outerSize(); ++k)
  {
    // Iterate over inside
    for(typename Eigen::SparseMatrix<T>::InnerIterator it (X,k); it; ++it)
    {
      if(it.col() == it.row())
      {
        V.coeffRef(it.col()) += it.value();
      }
    }
  }
}

template <typename T,typename DerivedV>
IGL_INLINE void igl::diag(
  const Eigen::SparseMatrix<T>& X, 
  Eigen::MatrixBase<DerivedV> & V)
{
  // Get size of input
  int m = X.rows();
  int n = X.cols();
  V.derived().resize((m>n?n:m),1);

  // Iterate over outside
  for(int k=0; k<X.outerSize(); ++k)
  {
    // Iterate over inside
    for(typename Eigen::SparseMatrix<T>::InnerIterator it (X,k); it; ++it)
    {
      if(it.col() == it.row())
      {
        V(it.col()) = it.value();
      }
    }
  }
}

template <typename T>
IGL_INLINE void igl::diag(
  const Eigen::SparseVector<T>& V,
  Eigen::SparseMatrix<T>& X)
{
  // clear and resize output
  Eigen::DynamicSparseMatrix<T, Eigen::RowMajor> dyn_X(V.size(),V.size());
  dyn_X.reserve(V.size());
  // loop over non-zeros
  for(typename Eigen::SparseVector<T>::InnerIterator it(V); it; ++it)
  {
    dyn_X.coeffRef(it.index(),it.index()) += it.value();
  }
  X = Eigen::SparseMatrix<T>(dyn_X);
}

template <typename T, typename DerivedV>
IGL_INLINE void igl::diag(
  const Eigen::MatrixBase<DerivedV> & V,
  Eigen::SparseMatrix<T>& X)
{
  assert(V.rows() == 1 || V.cols() == 1);
  // clear and resize output
  Eigen::DynamicSparseMatrix<T, Eigen::RowMajor> dyn_X(V.size(),V.size());
  dyn_X.reserve(V.size());
  // loop over non-zeros
  for(int i = 0;i<V.size();i++)
  {
    dyn_X.coeffRef(i,i) += V[i];
    i++;
  }
  X = Eigen::SparseMatrix<T>(dyn_X);
}

#ifndef IGL_HEADER_ONLY
// Explicit template specialization
template void igl::diag<double, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::SparseMatrix<double, 0, int> const&, Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
#endif