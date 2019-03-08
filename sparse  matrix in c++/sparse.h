/*
 * sparse.h
 *
 *  Created on: 31 ???? 2014
 *      Author: mktba1
 */

#ifndef SPARSE_H_
#define SPARSE_H_


#include <iostream>
#include <stdexcept>
#include "defs.h"
#include <vector>
#include <memory>

namespace mtm{
using std::vector;
using std::shared_ptr;

template<typename T>
class SparseMatrix{
public :
	class row_type;
private:
//	class Row;
	int rows;
	int coloumns;
	int nonZeroCells;
	vector<row_type> vectorOfRows;
public:



/************ class row_type ****************/

	class row_type{

/************* class row_type::Cell **********/
		class Cell{
			shared_ptr<T> element;
			Cell(int index):
				element(new T), index(index){
			}
			friend class SparseMatrix<T>;
		public:
			int index;
			const T& get() const{
				return *(this->element);
			}
		};

		vector<Cell> vectorOfCells;

		row_type(int index):
			index(index){
		}
		T& operator()(int c, bool* flag);
		const T& operator()(int col) const;
		row_type& operator+=(const row_type& row);
//		row_type& operator=(const row_type& second);

		friend class SparseMatrix<T>;

	public:
		int index;
		const row_type& get() const{
			return *this;
		}



/********** class row_type::const_iterator ******************/
		class const_iterator{
			typename vector<Cell>::const_iterator iterator;
			const_iterator(typename vector<Cell>::const_iterator it) :
				iterator(it){
			}
			friend class SparseMatrix<T> ;
		public :
			const_iterator& operator++() {
				try{
					++this->iterator;
				} catch (...) {
					++this->iterator;
				}
					return *this;
				}

			const_iterator operator+(int relative_index) {
				const_iterator it = *this;
				for (int i = 1; i <= relative_index && relative_index > 0; i++) {
					++it;
				}
				return it;
			}

			const_iterator operator++(int) {
				const_iterator savePaste = *this;
				++this->iterator;
				return savePaste;
			}

			bool operator==(const const_iterator& c_iterator) const {
				return (this->iterator == c_iterator.iterator);
			}

			bool operator!=(const const_iterator& c_iterator) const {
				return !(*this == c_iterator);
			}

			const Cell* operator->() const{
				return &(*this->iterator);
			}
		};

		const_iterator begin() const{
			return this->vectorOfCells.begin();
		}
		const_iterator end() const{
			return this->vectorOfCells.end();
		}
		row_type(const row_type& row){
					index = row.index;
					typename vector<Cell>::const_iterator it = row.vectorOfCells.begin();
						for (; it != row.vectorOfCells.end(); ++it) {
							const Cell& tmpCell = *it;
							vectorOfCells.push_back(Cell(tmpCell.index));
							Cell& tmpCell1 = vectorOfCells.at(vectorOfCells.size() - 1);
							*(tmpCell1.element) = *(tmpCell.element);
						}
				}
	};



	/************** main matrix methods *****************************/

	SparseMatrix(const SparseMatrix& matrix);
	SparseMatrix(int rows, int coloumns):
		rows(rows),coloumns(coloumns),nonZeroCells(0){
	}
	int get_row_count() const {
		return rows;
	}
	int get_col_count() const {
		return coloumns;
	}
	int get_non_zero_cells() const {
		return nonZeroCells;
	}
	T& operator()(int row, int col);
	T operator()(int r, int c) const;
	SparseMatrix operator+(const SparseMatrix& second_matrix) const;
	SparseMatrix& operator+=(const SparseMatrix<T>& second_matrix);
	SparseMatrix operator*(const SparseMatrix& second_matrix) const;

	bool isIn(int row , int col) const;

	/*****************row_const_iterator*******************************/

	class row_const_iterator {
		typename vector<SparseMatrix<T>::row_type>::const_iterator iterator;
			row_const_iterator(typename vector<row_type>::const_iterator iterator):
				iterator(iterator) {
			}
	    	friend class SparseMatrix;
	        friend class row_type;

	    public:
	    	row_const_iterator& operator++() {
	    		try{
	    			++this->iterator;
	    		} catch (...) {
	    			++this->iterator;
	    		}
	    		return *this;
	    	}

	    	row_const_iterator operator+(int relative_index) {
	    		row_const_iterator it = *this;
	    		for (int i = 1; i <= relative_index && relative_index > 0; i++) {
	    			++it;
	    		}
	    		return it;
	    	}

	    	row_const_iterator operator++(int) {
	    		row_const_iterator savePaste = *this;
	    		++this->iterator;
	    		return savePaste;
	    	}

	    	bool operator==(const row_const_iterator& const_iterator) const {
	    		return (this->iterator == const_iterator.iterator);
	    	}

	    	bool operator!=(const row_const_iterator& iterator) const {
	    		return !(*this == iterator);
	    	}

	    	const row_type* operator->() const{
	    		return &(*this->iterator);
	    	}
//	    	const row_type& get() const{
//	        	return *this->iterator;
//	    	}

	    };

	 row_const_iterator begin() const{
		return row_const_iterator(this->vectorOfRows.begin());
	 }
	 row_const_iterator end() const{
		return row_const_iterator(this->vectorOfRows.end());
	 }

};

///////////////////////////////////////////////////////////////////////


template<typename T>
T& SparseMatrix<T>::row_type::operator()(int c, bool* flag) {
	typename vector<Cell>::iterator it = vectorOfCells.begin();
	for (; it != vectorOfCells.end(); it++) {
		Cell& tmpCell = *it;
		if (tmpCell.index == c) {
			if (flag) {
				*flag = false;
			}
			return *(tmpCell.element);
		}
		if (tmpCell.index > c) {
			typename vector<Cell>::iterator it2 =
					vectorOfCells.insert(it,Cell(c));
			if (flag) {
				*flag = true;
			}
			Cell& tmp2 = *it2;
			return *(tmp2.element);
		}
	}
	vectorOfCells.push_back(Cell(c));
	Cell& tmp = vectorOfCells.at(vectorOfCells.size() - 1);
	if (flag) {
		*flag = true;
	}
	return *(tmp.element);
}

/////////////////////////////////////////////////////////////////////////
template<class T>
T& SparseMatrix<T>::operator()(int r, int c){
	if(r >= rows || c >= coloumns || r < 0 || c < 0){
		throw InvalidIndex();
	}

	typename vector<row_type>::iterator tmpIt = vectorOfRows.begin();
	for (;tmpIt != vectorOfRows.end(); ++tmpIt){
		row_type& row = *tmpIt;
		if (row.index == r) {
			bool flag = true;
			T& ret = row(c, &flag);
			if (flag) {
				nonZeroCells++;
			}
			return ret;
		}
		if (row.index > r) {
			typename vector<row_type>::iterator it1 = vectorOfRows.insert(tmpIt,row_type(r));
			row_type& tmp1 = *it1;
			nonZeroCells++;
			return tmp1(c, NULL);
		}
//
	}
	vectorOfRows.push_back(row_type(r));
	row_type& tmp = vectorOfRows.at(vectorOfRows.size() - 1);
	nonZeroCells++;
	return tmp(c, NULL);
}
///////////////////////////////////////////////////////////////////////////
template<class T>
const T& SparseMatrix<T>::row_type::operator()(int col) const {
	typename vector<Cell>::const_iterator it = vectorOfCells.begin();
	for (; it != vectorOfCells.end(); it++) {
		const Cell& tmp = *it;
		if (tmp.index == col) {
			return *(tmp.element);
		}

	}
	throw ElementDoesNotExist();
}

///////////////////////////////////////////////////////////////////////////
template<typename T>
T SparseMatrix<T>::operator()(int r, int c) const{
	if(r >= rows || c >= coloumns || c < 0 || r < 0){
		throw InvalidIndex();
	}
	typename vector<row_type>::const_iterator it = vectorOfRows.begin();
		for (; it != vectorOfRows.end(); ++it) {
			const row_type& tmpRow = *it;
			if (tmpRow.index == r) {
				return tmpRow(c);
			}
		}
		throw ElementDoesNotExist();
}
////////////////////////////////////////////////////////////////////////////
template<typename T>
typename SparseMatrix<T>::row_type& SparseMatrix<T>::row_type::operator+=(const row_type& row){
	typename vector<Cell>::iterator col_it1 = vectorOfCells.begin();
	typename vector<Cell>::const_iterator col_it2 = row.vectorOfCells.begin();
	while (col_it2 != row.vectorOfCells.end() || col_it1 != vectorOfCells.end()) {
		if (col_it1 == vectorOfCells.end()) {
			while (col_it2 != row.vectorOfCells.end()) {
				const Cell& tmpCell = *col_it2;
				vectorOfCells.push_back(Cell(tmpCell.index));
				Cell& tmpCell2 = vectorOfCells.at(vectorOfCells.size() - 1);
				*(tmpCell2.element) = *(*col_it2).element;
				++col_it2;
			}
			return *this;
		}
		if(col_it2 == row.vectorOfCells.end()){
			return *this;
		}
		if((*col_it2).index == (*col_it1).index){
			*(*col_it1).element += *(*col_it2).element;
			++col_it1;
			++col_it2;
			continue;
		}
		if((*col_it2).index < (*col_it1).index){
			typename vector<Cell>::iterator tmp_it = vectorOfCells.insert(col_it1,
								Cell((*col_it2).index));
//			Cell& tmp3 = *tmp_it;
			*(*tmp_it).element = *((*col_it2).element);
			++col_it2;
			continue;
		}
		++col_it1;
	}
	return *this;
}

////////////////////////////////////////////////////////////////////////////
//template<typename T>
//typename SparseMatrix<T>::row_type& SparseMatrix<T>::row_type::operator=(
//		const row_type& second) {
//	typename vector<Cell>::const_iterator iterator = second.vectorOfCells.begin();
//	while (iterator != second.vectorOfCells.end()){
//		vectorOfCells.push_back(Cell((*iterator).index));
//		Cell& cell1 = vectorOfCells.at(vectorOfCells.size() - 1);
//		*(cell1.element) = *(*iterator).element;
//		++iterator;
//	}
//	return *this;
//}
///////////////////////////////////////////////////////////////////////////
template<typename T>
SparseMatrix<T>& SparseMatrix<T>::operator+=(const SparseMatrix<T>& second_matrix){
	 if (rows != second_matrix.rows || coloumns != second_matrix.coloumns){
		 throw DimensionMismatch();
	 }
	 if (second_matrix.vectorOfRows.size() == 0){
		 return *this;
	 }
	 typename vector<row_type>::iterator row_it1 = vectorOfRows.begin();
	 typename vector<row_type>::const_iterator row_it2 =
			 second_matrix.vectorOfRows.begin();
	 while(row_it2 != second_matrix.vectorOfRows.end() ||
			 row_it1 != vectorOfRows.end()){
		 if (row_it1 == vectorOfRows.end()) {
		 	while (row_it2 != second_matrix.vectorOfRows.end()) {
		 		const row_type& tmpRow = *row_it2;
		 		vectorOfRows.push_back(row_type(tmpRow.index));
		 		row_type& tmpCell2 = vectorOfRows.at(vectorOfRows.size() - 1);
		 		tmpCell2 = *row_it2;
		 		++row_it2;
		 	}
		 	return *this;
		 }
		 if(row_it2 ==  second_matrix.vectorOfRows.end()){
			 return *this;
		 }
		 if((*row_it2).index == (*row_it1).index){
			*row_it1 += *row_it2;
		 	++row_it1;
		 	++row_it2;
		 	continue;
		 }
		 if((*row_it2).index < (*row_it1).index){
			 typename vector<row_type>::iterator tmp_it = vectorOfRows.insert(row_it1,
		 								row_type((*row_it2).index));
//		 	row_type& tmp3 = *tmp_it;
		 	*tmp_it = *row_it2;
		 	++row_it2;
		 	continue;
		 }
		 ++row_it1;
	 }
	 return *this;

}
///////////////////////////////////////////////////////////////////////////////
template<typename T>
SparseMatrix<T> SparseMatrix<T>::operator+(const SparseMatrix& second_matrix) const{
	SparseMatrix<T> newMat(get_row_count(), get_col_count());
	newMat = *this;
	newMat += second_matrix;
	return newMat;
}

////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
//	SparseMatrix operator*(const SparseMatrix& second_matrix) const;
template<typename T>
SparseMatrix<T>::SparseMatrix(const SparseMatrix& matrix){
	this->coloumns = matrix.coloumns;
	this->rows = matrix.rows;
	this->nonZeroCells = matrix.nonZeroCells;
	this->vectorOfRows = matrix.vectorOfRows;
}
//////////////////////////////////////////////////////////////////////////////
template<typename T>
SparseMatrix<T> SparseMatrix<T>::operator*(const SparseMatrix& second_matrix) const{
	if(coloumns != second_matrix.rows){
		throw DimensionMismatch();
	}
	SparseMatrix newMatrix(rows ,second_matrix.coloumns);
	T M1(0) , M2(0);
	for(int i=0; i<rows; i++){
		for(int j = 0; j<second_matrix.coloumns; j++){
			T sum(0);
			for(int k=0; k<coloumns; k++){
				try{
					M1 = this->operator ()(i,k);
					M2 = second_matrix(k,j);
				}catch (const mtm::ElementDoesNotExist&)
				{
					continue;
				}
				sum = sum + (M2 * M1) ;
			}
			if(sum != 0){
				newMatrix(i,j) = sum;

			}
		}
	}
	return newMatrix;
}

/////////////////////////////////////////////////////////////////////////////////
template<class T>
inline mtm::SparseMatrix<T> sparse_minor(const mtm::SparseMatrix<T>& matrix, int except_col){

	int row = matrix.get_row_count();
	row--;
	int coloumn = matrix.get_col_count();
	coloumn--;
	mtm::SparseMatrix<T> newMatrix(row ,coloumn);
	typename mtm::SparseMatrix<T>::row_const_iterator row_it=matrix.begin();

	for(; row_it != matrix.end(); ++row_it){

		if(row_it->index==0){
			continue;
		}
		typename mtm::SparseMatrix<T>::row_type row = row_it->get();
		typename mtm::SparseMatrix<T>::row_type::const_iterator col_it = row.begin();
		    for (; col_it != row.end();++col_it){
		    	if (col_it->index==except_col){
		    		continue;
		    		}
		    	else if (except_col < col_it->index){
		    		newMatrix(row_it->index-1,col_it->index-1) = matrix(row_it->index,col_it->index);
		    	}
		    	else{
		    		newMatrix(row_it->index-1,col_it->index) = matrix(row_it->index,col_it->index);
		    	}
		    }

	}
	return newMatrix;

}


template<class T>
inline T aux_sparse_determinant(const mtm::SparseMatrix<T>& matrix){
	T result;
	int row = matrix.get_row_count();
//	int coloumn = matrix.get_col_count();
	if(row == 2){
		T x;
		if(matrix.isIn(0,0) && matrix.isIn(1,1)){
			x = matrix(0,0) * matrix(1,1);
		}
		if(matrix.isIn(0,1) && matrix.isIn(1,0)){
			x = x - (matrix(0,1)*matrix(1,0));
		}
		return x;
	}
	typename mtm::SparseMatrix<T>::row_const_iterator row2 = matrix.begin();
	typename mtm::SparseMatrix<T>::row_type row_it = row2->get();
	typename mtm::SparseMatrix<T>::row_type::const_iterator col_it = row_it.begin();
	int l = 0 ;
		for (; col_it != row_it.end();++col_it){
			if(col_it->index % 2 == 0){
				l=1;
			}
			else {
				l=-1;
			}

			 mtm::SparseMatrix<T> minmatrix = sparse_minor(matrix, col_it->index);
			 if(l == 1){
				 result = result + aux_sparse_determinant(minmatrix) * matrix(0,col_it->index);
			 } else {
				 result = result - aux_sparse_determinant(minmatrix) * matrix(0,col_it->index);

			 }
//			double first = matrix(0,col_it->index);
//			*det += l*mindet;
		}
		return result;
}


template<class T>
inline T sparse_determinant(const mtm::SparseMatrix<T>& matrix){
	int row = matrix.get_row_count();
	int coloumn = matrix.get_col_count();
	if (row != coloumn){
		throw mtm::DimensionMismatch();
	}

	T det = aux_sparse_determinant(matrix);
	return det;
}



template<class T>
bool SparseMatrix<T>::isIn(int r , int c) const{
				row_const_iterator it = begin();
				for (; it != end(); ++it) {
					if (it->index == r) {
						 const row_type& row = it->get();
						 typename row_type::const_iterator col_it = row.begin();
						 for (; col_it != row.end();++col_it){
							 if(col_it->index == c){
									return true;
								}
							}

				}
		}
				return false;

}


}

#endif /* SPARSE_H_ */
