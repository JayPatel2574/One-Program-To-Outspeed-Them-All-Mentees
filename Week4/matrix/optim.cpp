#include "matrix.h"

matrix::matrix(unsigned long rowNum, unsigned long colNum){
/*     throw std::invalid_argument("CONSTRUCTOR NOT IMPLEMENTED!\n"); // Not optimisable
    data.resize(rowNum*colNum,0); */
    data.resize(rowNum*colNum);
    rows = rowNum;
    cols = colNum;

        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;

        unsigned long chunkSize = (rowNum + numThreads - 1) / numThreads; 

        for (unsigned int t = 0; t < numThreads; ++t) {
            unsigned long startRow = t * chunkSize;
            unsigned long endRow = std::min((t + 1) * chunkSize, rowNum);
            threads.emplace_back([this, startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < cols; ++j) {
                        data[i * cols + j] = 0; 
                    }
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

matrix::matrix(unsigned long size){
/*     throw std::invalid_argument("1D CONSTRUCTOR NOT IMPLEMENTED!\n"); // Not optimisable
    matrix(size,1); */
    data.resize(size);
    rows = size;
    cols = 1; 
        
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
    const unsigned long chunkSize = (size + numThreads - 1) / numThreads;

    for (unsigned int t = 0; t < numThreads; ++t) {
        unsigned long startRow = t * chunkSize;
        unsigned long endRow = std::min((t + 1) * chunkSize, size);
        threads.emplace_back([this, startRow, endRow]() {
            for (unsigned long i = startRow; i < endRow; ++i) {
                data[i] = 0;
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}



matrix::matrix(const matrix& other) {
/*     throw std::invalid_argument("COPY CONSTRUCTOR NOT IMPLEMENTED!\n"); // Not optimisable
    data = other.data; */
    data.resize(other.rows*other.cols);
    rows = other.rows;
    cols = other.cols;

        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        unsigned long chunkSize = (rows + numThreads - 1) / numThreads; 

        for (unsigned int t = 0; t < numThreads; ++t) {
            unsigned long startRow = t * chunkSize;
            unsigned long endRow = std::min((t + 1) * chunkSize, rows);
            threads.emplace_back([this,&other , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < cols; ++j) {
                        data[i * cols + j] = other.data[i * cols + j]; 
                    }
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }
    
}

matrix& matrix::operator=(const matrix& other) {
/*     throw std::invalid_argument("EQUALITY OPERATOR NOT IMPLEMENTED!\n"); // Not optimisable */
    // Allocate new resource
    rows = other.rows;
    cols = other.cols;
/*     data = other.data; */

        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        unsigned long chunkSize = (rows + numThreads - 1) / numThreads; 

        for (unsigned int t = 0; t < numThreads; ++t) {
            unsigned long startRow = t * chunkSize;
            unsigned long endRow = std::min((t + 1) * chunkSize, rows);
            threads.emplace_back([this,&other ,startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < cols; ++j) {
                        data[i * cols + j] = other.data[i * cols + j]; 
                    }
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }
        
    return *this;
}

matrix operator+(const matrix& first, const matrix& second){
/*     throw std::invalid_argument("ELEMENTWISE ADDITION OPERATOR NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    if (first.rows!=second.rows || first.cols!=second.cols){
        throw std::invalid_argument("cannot add ( "+ to_string(first.rows) +" , " + to_string(first.cols) + " ) with ( " + to_string(second.rows) + " , " + to_string(second.cols) + " )" );
        }
    else{
/*         matrix sum(first.rows,first.cols);
        for( unsigned long i=0;i<first.rows*first.cols;i++){
            sum.data[i]=first.data[i]+second.data[i];
        } */
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        unsigned long chunkSize = (first.rows + numThreads - 1) / numThreads;
        matrix sum(first.rows,first.cols);

        for (unsigned int t = 0; t < numThreads; ++t) {
            unsigned long startRow = t * chunkSize;
            unsigned long endRow = std::min((t + 1) * chunkSize, first.rows);
            threads.emplace_back([&first, &second, &sum , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < first.cols; ++j) {
                        sum.data[i * first.cols + j] = first.data[i * first.cols + j]+second.data[i * first.cols + j]; 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }    

        return sum;
    }
}

matrix operator-(const matrix& first, const matrix& second){
    /* throw std::invalid_argument("ELEMENTWISE SUBTRACTION OPERATOR NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    if (first.rows!=second.rows || first.cols!=second.cols){
        throw std::invalid_argument("cannot add ( "+ to_string(first.rows) +" , " + to_string(first.cols) + " ) with ( " + to_string(second.rows) + " , " + to_string(second.cols) + " )" );
        }
    else{
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;        
        unsigned long chunkSize = (first.rows + numThreads - 1) / numThreads;
        matrix sum(first.rows,first.cols);
/*         for( unsigned long i=0;i<first.rows*first.cols;i++){
            sum.data[i]=first.data[i]-second.data[i];
        } */

        for (unsigned int t = 0; t < numThreads; ++t) {
            unsigned long startRow = t * chunkSize;
            unsigned long endRow = std::min((t + 1) * chunkSize, first.rows);
            threads.emplace_back([&first, &second, &sum , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < first.cols; ++j) {
                        sum.data[i * first.cols + j] = first.data[i * first.cols + j]-second.data[i * first.cols + j]; 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }

        return sum;
    }
}

matrix operator*(const matrix& first, const matrix& second){
    /* throw std::invalid_argument("ELEMENTWISE MULTIPLICATION OPERATOR NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    if (first.rows!=second.rows || first.cols!=second.cols){
        throw std::invalid_argument("cannot add ( "+ to_string(first.rows) +" , " + to_string(first.cols) + " ) with ( " + to_string(second.rows) + " , " + to_string(second.cols) + " )" );
        }
    else{

        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        unsigned long chunkSize = (first.rows + numThreads - 1) / numThreads;
        matrix sum(first.rows,first.cols);
/*         for( unsigned long i=0;i<first.rows*first.cols;i++){
            sum.data[i]=first.data[i]*second.data[i];
        } */
        for (unsigned int t = 0; t < numThreads; ++t) {
            unsigned long startRow = t * chunkSize;
            unsigned long endRow = std::min((t + 1) * chunkSize, first.rows);
            threads.emplace_back([&first, &second, &sum , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < first.cols; ++j) {
                        sum.data[i * first.cols + j] = first.data[i * first.cols + j]*second.data[i * first.cols + j]; 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }

        return sum;
    }
}

matrix operator/(const matrix& first, const matrix& second){
    /* throw std::invalid_argument("ELEMENTWISE DIVISION OPERATOR NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    if (first.rows!=second.rows || first.cols!=second.cols){
        throw std::invalid_argument("cannot add ( "+ to_string(first.rows) +" , " + to_string(first.cols) + " ) with ( " + to_string(second.rows) + " , " + to_string(second.cols) + " )" );
        }
    else{
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;        
        unsigned long chunkSize = (first.rows + numThreads - 1) / numThreads;
        matrix sum(first.rows,first.cols);
/*         for( unsigned long i=0;i<first.rows*first.cols;i++){
            sum.data[i]=first.data[i]/second.data[i];
        } */
        for (unsigned int t = 0; t < numThreads; ++t) {
            unsigned long startRow = t * chunkSize;
            unsigned long endRow = std::min((t + 1) * chunkSize, first.rows);
            threads.emplace_back([&first, &second, &sum , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < first.cols; ++j) {
                        sum.data[i * first.cols + j] = first.data[i * first.cols + j]/second.data[i * first.cols + j]; 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }

        return sum;
    }
}

matrix operator*(const matrix& first, const double t) {
    /* throw std::invalid_argument("FLOAT MULTIPLICATION OPERATOR NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;   
    unsigned long chunkSize = (first.rows + numThreads - 1) / numThreads;
    matrix sum(first.rows, first.cols);
/*     for (unsigned long i=0; i<first.rows*first.cols; i++) {
        sum.data[i] = first.data[i]*t;
    } */
        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, first.rows);
            threads.emplace_back([&first, &t, &sum , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < first.cols; ++j) {
                        sum.data[i * first.cols + j] = first.data[i * first.cols + j]*t; 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }

    return sum;
}

matrix operator+(const matrix& first, const double t) {
    /* throw std::invalid_argument("FLOAT ADDITION OPERATOR NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;   
    unsigned long chunkSize = (first.rows + numThreads - 1) / numThreads;
    matrix sum(first.rows, first.cols);
/*     for (unsigned long i=0; i<first.rows*first.cols; i++) {
        sum.data[i] = first.data[i]+t;
    } */
        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, first.rows);
            threads.emplace_back([&first, &t, &sum , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < first.cols; ++j) {
                        sum.data[i * first.cols + j] = first.data[i * first.cols + j]+t; 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }

    return sum;
}

matrix operator-(const matrix& first, const double t) {
    /* throw std::invalid_argument("FLOAT SUBTRACTION OPERATOR NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;    
    unsigned long chunkSize = (first.rows + numThreads - 1) / numThreads;
    matrix sum(first.rows, first.cols);
    /* for (unsigned long i=0; i<first.rows*first.cols; i++) {
        sum.data[i] = first.data[i]-t;
    } */
        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, first.rows);
            threads.emplace_back([&first, &t, &sum , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < first.cols; ++j) {
                        sum.data[i * first.cols + j] = first.data[i * first.cols + j]-t; 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }

    return sum;
}

matrix operator/(const matrix& first, const double t) {
    /* throw std::invalid_argument("FLOAT DIVISION OPERATOR NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;    
    unsigned long chunkSize = (first.rows + numThreads - 1) / numThreads;
    matrix sum(first.rows, first.cols);
/*     for (unsigned long i=0; i<first.rows*first.cols; i++) {
        sum.data[i] = first.data[i]/t;
    } */
        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, first.rows);
            threads.emplace_back([&first, &t, &sum , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < first.cols; ++j) {
                        sum.data[i * first.cols + j] = first.data[i * first.cols + j]/t; 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }

    return sum;
}

matrix matmul(const matrix& first, const matrix& second){
    /* throw std::invalid_argument("MATMUL FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    pair<unsigned long, unsigned long> dim1 = first.shape();
    pair<unsigned long, unsigned long> dim2 = second.shape();

    if( dim1.second != dim2.first){
        throw std::invalid_argument("cannot matmul ( "+ to_string(dim1.first) +" , " + to_string(dim1.second) + " ) with ( " + to_string(dim2.first) + " , " + to_string(dim2.second) + " )" );
    }
    else{
        matrix net(dim1.first,dim2.second);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
    unsigned long chunkSize = (dim1.first + numThreads - 1) / numThreads;

        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, dim1.first);
            threads.emplace_back([&first, &second, &dim1, &dim2, &net, startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
            for(unsigned long j=0;j< dim2.second;j++){
                double sum=0;
                for(unsigned long k=0;k< dim1.second;k++){
                    sum+=first(i,k)*second(k,j);
                }
                net(i,j)=sum;
            }
        }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }
        return net;
    }
}

matrix zeros(unsigned long rows, unsigned long cols){
    /* throw std::invalid_argument("ZEROS FUNCTION NOT IMPLEMENTED!\n"); */ // you can maybe optimise this
    return matrix(rows,cols);
}

matrix zeros(unsigned long size){
    /* throw std::invalid_argument("1D ZEROS FUNCTION NOT IMPLEMENTED!\n"); */ // you can maybe optimise this
    return matrix(size);
}

matrix eye(unsigned long size){
    /* throw std::invalid_argument("1D EYE FUNCTION NOT IMPLEMENTED!\n"); */ // you cant optimise this
    matrix diag(size,size);
    for(int i=0;i<size;i++){
        diag(i,i)=1;
    }
    return diag;
}

matrix eye(unsigned long rows, unsigned long cols){
    /* throw std::invalid_argument("EYE FUNCTION NOT IMPLEMENTED!\n"); */ // you cant optimise this
    matrix diag(rows,cols);
    for(int i=0;i<min(rows,cols);i++){
        diag(i,i)=1;
    }
    return diag;
}

matrix identity(unsigned long size){
    /* throw std::invalid_argument("IDENTITY FUNCTION NOT IMPLEMENTED!\n"); */ // you cant optimise this
    return eye(size);
}

matrix max(matrix &arr,int axis) {
    /* throw std::invalid_argument("SPECIFIC MAX FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    if (axis < 0 || axis > 1) throw std::invalid_argument("Axis must be 0 or 1");
    
    double twoRows,twoCols;
    twoRows = arr.shape().first;
    twoCols = arr.shape().second;

    matrix result(axis == 0 ? 1 : twoRows, axis == 0 ? twoCols : 1);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        
    if (axis == 0) {  //largest each col
    unsigned long chunkSize = (twoCols + numThreads - 1) / numThreads; 
        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startcol = t * chunkSize;
            unsigned long endcol = std::min((t + 1) * chunkSize, arr.cols);            
            threads.emplace_back([&arr, &result, twoRows, startcol, endcol](){
                for(unsigned int i = startcol; i<endcol; ++i){
                    double max_value = arr(0, i);
                    for (unsigned long j = 1; j < twoRows; ++j) {
                        if (arr(j, i) > max_value) {
                            max_value = arr(j, i);
                        }
                    }
                    result(0, i) = max_value;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }        
    } else {  //largest for each row
    unsigned long chunkSize = (twoRows + numThreads - 1) / numThreads;
        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startrow = t * chunkSize;
            unsigned long endrow = std::min((t + 1) * chunkSize, arr.rows);            
            threads.emplace_back([&arr, &result, twoCols, startrow, endrow](){
                for(unsigned int i = startrow; i<endrow; ++i){
                    double max_value = arr(i, 0);
                    for (unsigned long j = 1; j < twoCols; ++j) {
                        if (arr(i, j) > max_value) {
                            max_value = arr(i, j);
                        }
                    }
                    result(i, 0) = max_value;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }        
    }

    return result;
}

matrix argmax(matrix &arr,int axis) {
    /* throw std::invalid_argument("SPECIFIC ARGMAX FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    if (axis < 0 || axis > 1) throw std::invalid_argument("Axis must be 0 or 1");
    
    double twoRows,twoCols;
    twoRows = arr.shape().first;
    twoCols = arr.shape().second;

    matrix result(axis == 0 ? 1 : twoRows, axis == 0 ? twoCols : 1);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        
    if (axis == 0) {  //largest each col
    unsigned long chunkSize = (twoCols + numThreads - 1) / numThreads; 
        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startcol = t * chunkSize;
            unsigned long endcol = std::min((t + 1) * chunkSize, arr.cols);            
            threads.emplace_back([&arr, &result, twoRows, startcol, endcol](){
                for(unsigned int i = startcol; i<endcol; ++i){
                    double max_value = arr(0, i);
                    unsigned long int max_index = 0;
                    for (unsigned long j = 1; j < twoRows; ++j) {
                        if (arr(j, i) > max_value) {
                            max_value = arr(j, i);
                            max_index = j;
                        }
                    }
                    result(0, i) = max_index;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }        
    } else {  //largest for each row
    unsigned long chunkSize = (twoRows + numThreads - 1) / numThreads;
        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startrow = t * chunkSize;
            unsigned long endrow = std::min((t + 1) * chunkSize, arr.rows);            
            threads.emplace_back([&arr, &result, twoCols, startrow, endrow](){
                for(unsigned int i = startrow; i<endrow; ++i){
                    double max_value = arr(i, 0);
                    unsigned long int max_index = 0;
                    for (unsigned long j = 1; j < twoCols; ++j) {
                        if (arr(i, j) > max_value) {
                            max_value = arr(i, j);
                            max_index = j;
                        }
                    }
                    result(i, 0) = max_index;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }        
    }

    return result;
}

matrix max (matrix &arr) {
    /* throw std::invalid_argument("GENERAL MAX FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    double arrRows, arrCols;
    arrRows = arr.shape().first;
    arrCols = arr.shape().second;
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;

    if (arrRows != 1 && arrCols != 1) {
        matrix result(1, arrCols);
        unsigned long chunkSize = (arrCols + numThreads - 1) / numThreads;

        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startcol = t * chunkSize;
            unsigned long endcol = std::min((t + 1) * chunkSize, arr.cols);            
            threads.emplace_back([&arr, &result, arrRows, startcol, endcol](){
                for(unsigned int i = startcol; i<endcol; ++i){
                    double max_value = arr(0, i);
                    for (unsigned long j = 1; j < arrRows; ++j) {
                        if (arr(j, i) > max_value) {
                            max_value = arr(j, i);
                        }
                    }
                    result(0, i) = max_value;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }

        return result;
    }
    else if (arrRows == 1) {
        matrix t(1,1);
        t(0,0) = arr(0,0);
        for (unsigned long col = 1; col < arrCols; ++col) {
            t(0,0) = max(t(0,0),arr(0,col));
        }
        return t;
    }
    else {
        matrix t(1,1);
        t(0,0) = arr(0,0);
        for (unsigned long row = 1; row < arrRows; ++row) {
            t(0,0) = max(t(0,0),arr(row,0));
        }
        return t;
    }

}


matrix argmax (matrix &arr) {
    /* throw std::invalid_argument("GENERAL ARGMAX FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    double arrRows, arrCols;
    arrRows = arr.shape().first;
    arrCols = arr.shape().second;
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;

    if (arrRows != 1 && arrCols != 1) {
        matrix result(1, arrCols);
        unsigned long chunkSize = (arrCols + numThreads - 1) / numThreads;

        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startcol = t * chunkSize;
            unsigned long endcol = std::min((t + 1) * chunkSize, arr.cols);            
            threads.emplace_back([&arr, &result, arrRows, startcol, endcol](){
                for(unsigned int i = startcol; i<endcol; ++i){
                    double max_value = arr(0, i);
                    unsigned long max_index = 0;
                    for (unsigned long j = 1; j < arrRows; ++j) {
                        if (arr(j, i) > max_value) {
                            max_value = arr(j, i);
                            max_index = j;
                        }
                    }
                    result(0, i) = max_index;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }

        return result;
    }
    else if (arrRows == 1) {
        matrix t(1,1);
        t(0,0) = 0;
        for (unsigned long col = 1; col < arrCols; ++col) {
            if (arr(0,t(0,0)) < arr(col)) {
                t(0,0) = col;
            }
        }
        return t;
    }
    else {
        matrix t(1,1);
        t(0,0) = 0;
        for (unsigned long row = 1; row < arrRows; ++row) {
            if (arr(t(0,0),0) < arr(row)) {
                t(0,0) = row;
            }
        }
        return t;
    }

}

matrix min(matrix &arr,int axis) {
    /* throw std::invalid_argument("SPECIFIC MIN FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    if (axis < 0 || axis > 1) throw std::invalid_argument("Axis must be 0 or 1");
    
    double twoRows,twoCols;
    twoRows = arr.shape().first;
    twoCols = arr.shape().second;

    matrix result(axis == 0 ? 1 : twoRows, axis == 0 ? twoCols : 1);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        
    if (axis == 0) {  //smallest each col
    unsigned long chunkSize = (twoCols + numThreads - 1) / numThreads; 
        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startcol = t * chunkSize;
            unsigned long endcol = std::min((t + 1) * chunkSize, arr.cols);            
            threads.emplace_back([&arr, &result, twoRows, startcol, endcol](){
                for(unsigned int i = startcol; i<endcol; ++i){
                    double min_value = arr(0, i);
                    for (unsigned long j = 1; j < twoRows; ++j) {
                        if (arr(j, i) < min_value) {
                            min_value = arr(j, i);
                        }
                    }
                    result(0, i) = min_value;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }        
    } else {  //smallest for each row
    unsigned long chunkSize = (twoRows + numThreads - 1) / numThreads;
        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startrow = t * chunkSize;
            unsigned long endrow = std::min((t + 1) * chunkSize, arr.rows);            
            threads.emplace_back([&arr, &result, twoCols, startrow, endrow](){
                for(unsigned int i = startrow; i<endrow; ++i){
                    double min_value = arr(i, 0);
                    for (unsigned long j = 1; j < twoCols; ++j) {
                        if (arr(i, j) < min_value) {
                            min_value = arr(i, j);
                        }
                    }
                    result(i, 0) = min_value;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }        
    }

    return result;
}

matrix argmin(matrix &arr,int axis) {
    /* throw std::invalid_argument("SPECIFIC ARGMIN FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    if (axis < 0 || axis > 1) throw std::invalid_argument("Axis must be 0 or 1");
    
    double twoRows,twoCols;
    twoRows = arr.shape().first;
    twoCols = arr.shape().second;

    matrix result(axis == 0 ? 1 : twoRows, axis == 0 ? twoCols : 1);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        
    if (axis == 0) {  //smallest each col
    unsigned long chunkSize = (twoCols + numThreads - 1) / numThreads; 
        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startcol = t * chunkSize;
            unsigned long endcol = std::min((t + 1) * chunkSize, arr.cols);            
            threads.emplace_back([&arr, &result, twoRows, startcol, endcol](){
                for(unsigned int i = startcol; i<endcol; ++i){
                    double min_value = arr(0, i);
                    unsigned long int min_index = 0;
                    for (unsigned long j = 1; j < twoRows; ++j) {
                        if (arr(j, i) < min_value) {
                            min_value = arr(j, i);
                            min_index = j;
                        }
                    }
                    result(0, i) = min_index;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }        
    } else {  //smallest for each row
    unsigned long chunkSize = (twoRows + numThreads - 1) / numThreads;
        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startrow = t * chunkSize;
            unsigned long endrow = std::min((t + 1) * chunkSize, arr.rows);            
            threads.emplace_back([&arr, &result, twoCols, startrow, endrow](){
                for(unsigned int i = startrow; i<endrow; ++i){
                    double min_value = arr(i, 0);
                    unsigned long int min_index = 0;
                    for (unsigned long j = 1; j < twoCols; ++j) {
                        if (arr(i, j) < min_value) {
                            min_value = arr(i, j);
                            min_index = j;
                        }
                    }
                    result(i, 0) = min_index;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }        
    }

    return result;
}

matrix min (matrix &arr) {
    /* throw std::invalid_argument("GENERAL MIN FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    double arrRows, arrCols;
    arrRows = arr.shape().first;
    arrCols = arr.shape().second;
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;

    if (arrRows != 1 && arrCols != 1) {
        matrix result(1, arrCols);
        unsigned long chunkSize = (arrCols + numThreads - 1) / numThreads;

        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startcol = t * chunkSize;
            unsigned long endcol = std::min((t + 1) * chunkSize, arr.cols);            
            threads.emplace_back([&arr, &result, arrRows, startcol, endcol](){
                for(unsigned int i = startcol; i<endcol; ++i){
                    double min_value = arr(0, i);
                    for (unsigned long j = 1; j < arrRows; ++j) {
                        if (arr(j, i) < min_value) {
                            min_value = arr(j, i);
                        }
                    }
                    result(0, i) = min_value;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }

        return result;
    }
    else if (arrRows == 1) {
        matrix t(1,1);
        t(0,0) = arr(0,0);
        for (unsigned long col = 1; col < arrCols; ++col) {
            t(0,0) = min(t(0,0),arr(0,col));
        }
        return t;
    }
    else {
        matrix t(1,1);
        t(0,0) = arr(0,0);
        for (unsigned long row = 1; row < arrRows; ++row) {
            t(0,0) = min(t(0,0),arr(row,0));
        }
        return t;
    }

}


matrix argmin (matrix &arr) {
    /* throw std::invalid_argument("GENERAL ARGMIN FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    double arrRows, arrCols;
    arrRows = arr.shape().first;
    arrCols = arr.shape().second;
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;

    if (arrRows != 1 && arrCols != 1) {
        matrix result(1, arrCols);
        unsigned long chunkSize = (arrCols + numThreads - 1) / numThreads;

        for (unsigned long t = 0; t < numThreads; ++t) {
            unsigned long startcol = t * chunkSize;
            unsigned long endcol = std::min((t + 1) * chunkSize, arr.cols);            
            threads.emplace_back([&arr, &result, arrRows, startcol, endcol](){
                for(unsigned int i = startcol; i<endcol; ++i){
                    double min_value = arr(0, i);
                    unsigned long min_index = 0;
                    for (unsigned long j = 1; j < arrRows; ++j) {
                        if (arr(j, i) < min_value) {
                            min_value = arr(j, i);
                            min_index = j;
                        }
                    }
                    result(0, i) = min_index;    
                }
            });
        }
        for (auto& thread : threads) {
            thread.join();
        }

        return result;
    }
    else if (arrRows == 1) {
        matrix t(1,1);
        t(0,0) = 0;
        for (unsigned long col = 1; col < arrCols; ++col) {
            if (arr(0,t(0,0)) > arr(col)) {
                t(0,0) = col;
            }
        }
        return t;
    }
    else {
        matrix t(1,1);
        t(0,0) = 0;
        for (unsigned long row = 1; row < arrRows; ++row) {
            if (arr(t(0,0),0) > arr(row)) {
                t(0,0) = row;
            }
        }
        return t;
    }

}


matrix ones (unsigned long rows, unsigned long cols) {
    /* throw std::invalid_argument("ONES FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    matrix t(rows,cols);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;    
    unsigned long chunkSize = (rows + numThreads - 1) / numThreads;    

        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, rows);
            threads.emplace_back([&t , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < t.cols; ++j) {
                        t.data[i * t.cols + j] = 1; 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }
    return t;
}

matrix fabs(matrix &a) {
    /* throw std::invalid_argument("ELEMENTWISE FABS FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    matrix res(a.rows,a.cols);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;    
    unsigned long chunkSize = (res.rows + numThreads - 1) / numThreads;    

        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, res.rows);
            threads.emplace_back([&res, &a , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < res.cols; ++j) {
                        res.data[i * res.cols + j] = std::fabs(a.data[i * res.cols + j]); 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }
    return res;
}

matrix exp(matrix &a) {
    /* throw std::invalid_argument("ELEMENTWISE EXP FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    matrix res(a.rows,a.cols);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;    
    unsigned long chunkSize = (res.rows + numThreads - 1) / numThreads;    

        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, res.rows);
            threads.emplace_back([&res, &a , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < res.cols; ++j) {
                        res.data[i * res.cols + j] = std::exp(a.data[i * res.cols + j]); 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }
    return res;
}

matrix tanh(matrix &a) {
    /* throw std::invalid_argument("ELEMENTWISE TANH FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    matrix res(a.rows,a.cols);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;    
    unsigned long chunkSize = (res.rows + numThreads - 1) / numThreads;    

        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, res.rows);
            threads.emplace_back([&res, &a , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < res.cols; ++j) {
                        res.data[i * res.cols + j] = std::tanh(a.data[i * res.cols + j]); 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }
    return res;
}

matrix log(matrix &a, double logbase) {
    /* throw std::invalid_argument("ELEMENTWISE LOG FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    matrix res(a.rows,a.cols);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;    
    unsigned long chunkSize = (res.rows + numThreads - 1) / numThreads;    

        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, res.rows);
            threads.emplace_back([&res, &a , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < res.cols; ++j) {
                        res.data[i * res.cols + j] = std::log(a.data[i * res.cols + j]); 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }
    return res;
}

matrix sqrt(matrix &a) {
    /* throw std::invalid_argument("ELEMENTWISE SQRT FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    matrix res(a.rows,a.cols);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;    
    unsigned long chunkSize = (res.rows + numThreads - 1) / numThreads;    

        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, res.rows);
            threads.emplace_back([&res, &a , startRow, endRow]() {
                for (unsigned long i = startRow; i < endRow; ++i) {
                    for (unsigned long j = 0; j < res.cols; ++j) {
                        res.data[i * res.cols + j] = std::sqrt(a.data[i * res.cols + j]); 
                    }
                }
            });
        } 
        for (auto& thread : threads) {
            thread.join();
        }
    return res;  
}

matrix matrix::inverse(){
    /* throw std::invalid_argument("MATRIX INVERSE FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    matrix a = *this;
    pair<unsigned long, unsigned long> dim = a.shape();
    if (dim.first != dim.second) 
        throw std::invalid_argument("Cannot invert ( "+ to_string(dim.first) +" , " + to_string(dim.second) + " )");
    unsigned long n = a.rows;
    matrix augmented(n, 2 * n);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads1;
        unsigned long chunkSize = (n + numThreads - 1) / numThreads;
        // Initialize the augmented matrix with the identity matrix on the right
    for (unsigned int k = 0; k < numThreads; ++k) {
        unsigned long startRow = k * chunkSize;
        unsigned long endRow = std::min((k + 1) * chunkSize, n);
        threads1.emplace_back([&augmented, &a, n, startRow, endRow]() {
        for (unsigned long i = startRow; i < endRow; ++i) {
            for (unsigned long j = 0; j < n; ++j) {
                augmented(i, j) = a(i, j);
                augmented(i, j + n) = (i == j) ? 1.0 : 0.0;
            }
        }
        });
    }
        for (auto& thread : threads1) {
            thread.join();
        }
            // Perform Gauss-Jordan elimination
            std::vector<std::thread> threads2;
    for (unsigned int k = 0; k < numThreads; ++k){
        unsigned long startRow = k * chunkSize;
        unsigned long endRow = std::min((k + 1) * chunkSize, n);
        threads2.emplace_back([&augmented, &a, n, startRow, endRow]() {
        for (unsigned long i = startRow; i < endRow; ++i) {
                // Find the pivot
                double pivot = augmented(i, i);
                if (pivot == 0.0) {
                    throw runtime_error("Matrix is singular and cannot be inverted.");
                }

                // Normalize the pivot row
                for (unsigned long j = 0; j < 2 * n; ++j) {
                    augmented(i, j) /= pivot;
                }

                // Eliminate the current column in other rows
                for (unsigned long k = 0; k < n; ++k) {
                    if (k != i) {
                        double factor = augmented(k, i);
                        for (unsigned long j = 0; j < 2 * n; ++j) {
                            augmented(k, j) -= factor * augmented(i, j);
                        }
                    }
                }
            }
        });
    }
        for (auto& thread : threads2) {
            thread.join();
        }
            // Extract the inverse matrix from the augmented matrix
            matrix result(n, n);
            std::vector<std::thread> threads3;
    for (unsigned int k = 0; k < numThreads; ++k){
        unsigned long startRow = k * chunkSize;
        unsigned long endRow = std::min((k + 1) * chunkSize, n);
        threads3.emplace_back([&augmented, &a, n, &result, startRow, endRow]() {        
            for (unsigned long i = startRow; i < endRow; ++i) {
                for (unsigned long j = 0; j < n; ++j) {
                    result(i, j) = augmented(i, j + n);
                }
            }
        });    
    }
        for (auto& thread : threads3) {
            thread.join();
        }
    return result;        
}

matrix matrix::transpose(){
    /* throw std::invalid_argument("MATRIX TRANSPOSE FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    pair<unsigned long,unsigned long> dim = this->shape();
    matrix T(dim.second,dim.first);
        const unsigned int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;    
        unsigned long chunkSize = (dim.first + numThreads - 1) / numThreads;
        for (unsigned int k = 0; k < numThreads; ++k) {
            unsigned long startRow = k * chunkSize;
            unsigned long endRow = std::min((k + 1) * chunkSize, dim.first);
            threads.emplace_back([&T ,this ,startRow ,endRow ](){         
                for (int i = startRow ; i < endRow ; i++){
                    for (int j = 0 ; j < this->cols ; j++){
                        T(j,i) = this->data[i*this->cols + j];
                    }
                }
            });
        }
            for (auto& thread : threads) {
            thread.join();
        }
    return T;
}

double matrix::determinant(){
    /* throw std::invalid_argument("MATRIX DETERMINANT FUNCTION NOT IMPLEMENTED!\n"); */ // you can definitely optimise this
    if (rows != cols) {
        throw std::invalid_argument("Matrix must be square to calculate determinant");
    }
    unsigned long n = rows;
    matrix a(*this); // Make a copy of the matrix

    double det = 1;
    const unsigned int numThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    unsigned long chunkSize = (n + numThreads - 1) / numThreads; 

    for (unsigned int k = 0; k < numThreads; ++k) {
        unsigned long startRow = k * chunkSize;
        unsigned long endRow = std::min((k + 1) * chunkSize, n);
        threads.emplace_back([&a ,this ,n ,&det ,startRow ,endRow ](){   
            for (unsigned long i = startRow; i < endRow; ++i) {
                // Find the pivot
                unsigned long pivot = i;
                for (unsigned long j = i + 1; j < n; ++j) {
                    if (abs(a.data[j * n + i]) > abs(a.data[pivot * n + i])) {
                        pivot = j;
                    }
                }

                // Swap rows if needed
                if (pivot != i) {
                    for (unsigned long k = 0; k < n; ++k) {
                        std::swap(a.data[i * n + k], a.data[pivot * n + k]);
                    }
                    det *= -1; // Swap changes the sign of the determinant
                }

                // Check for zero pivot
                if (a.data[i * n + i] == 0) {
                    return 0; // Determinant is zero
                }

                // Eliminate the column
                for (unsigned long j = i + 1; j < n; ++j) {
                    double factor = a.data[j * n + i] / a.data[i * n + i];
                    for (unsigned long k = i; k < n; ++k) {
                        a.data[j * n + k] -= factor * a.data[i * n + k];
                    }
                }

                // Multiply the diagonal elements
                det *= a.data[i * n + i];
            }
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }

    return det;
}
