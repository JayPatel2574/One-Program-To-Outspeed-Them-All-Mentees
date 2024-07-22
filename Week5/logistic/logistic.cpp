#include "logistic.h"
#include "../matrix/matrix.h"

LogisticRegression::LogisticRegression(uint64_t D){
    d = D;
    weights = zeros(d,1);
    bias = 0;
    epsilon = EPS;
    eta = ETA;
}

matrix LogisticRegression::sigmoid(matrix z){
    matrix g(z.rows,1);
    g = 1.0*ones(z.rows,1);
    // Implement sigmoid function here as defined in README.md
    for(uint64_t i =0; i<z.rows;++i){
        g(i,0) = 1/(1+double(exp(double(z(i,0)))));
    }    
    return g;
}

double LogisticRegression::logisticLoss(matrix X, matrix Y){
    matrix Y_pred(X.shape().first,1);
    Y_pred = sigmoid((matmul(X,weights) + bias));
    __size d1 = Y.shape(), d2 = Y_pred.shape();
    if (d1 != d2){
        throw std::invalid_argument("Cannot compute loss of vectors with dimensions ( "+to_string(d1.first)+" , "
        +to_string(d1.second)+" ) and ( "+to_string(d2.first)+" , "+to_string(d2.second)+" ) do not match");
    }
    double loss = 0;
    // Compute the log loss as defined in README.md
    uint64_t n = Y.shape().first;
    for(uint64_t i=0 ; i<n; ++i){
        loss = loss -(Y(i,0)*double(log(Y_pred(i,0)))+(1-Y(i,0))*double(log(1-Y_pred(i,0))));
    }
    loss = loss/double(n);    
    return loss;
}

pair<matrix, double> LogisticRegression::lossDerivative(matrix X, matrix Y){
    matrix Y_pred(X.shape().first,1);
    Y_pred = sigmoid((matmul(X,weights) + bias));
    __size d1 = Y.shape(), d2 = Y_pred.shape();
    if (d1 != d2){
        throw std::invalid_argument("Cannot compute loss derivative of vectors with dimensions ( "+to_string(d1.first)+" , "
        +to_string(d1.second)+" ) and ( "+to_string(d2.first)+" , "+to_string(d2.second)+" ) do not match");
    }
    //Compute gradients as defined in README.md
    matrix dw(d,1);
    double db; 
    uint64_t n = Y.shape().first;
    for(uint64_t i=0; i<n ; ++i){
        double z = Y_pred(i,0) - Y(i,0);
        db += z;
        for(uint64_t j =0; j<d ; ++j){
            dw(j,0) += z*X(i,j);
        }    
    }
    db = db*(double(2)/n);
    dw = dw*(double(2)/n);    
    return {dw,db};
}

matrix LogisticRegression::predict(matrix X){
    matrix Y_pred3(X.shape().first,1);
    // Using the weights and bias, find the values of y for every x in X
    Y_pred3 = sigmoid((matmul(X,weights) + bias));
    for(uint64_t i=0; i<X.shape().first; ++i){
        if(Y_pred3(i,0)>=0.5){
            Y_pred3(i,0)=1;
        }
        else {
            Y_pred3(i,0)=0;
        }
    }
    return Y_pred3;
}  

void LogisticRegression::GD(matrix X, matrix Y,double learning_rate, uint64_t limit){
    eta = learning_rate;
    double old_loss = 0,loss = logisticLoss(X,Y);
    train_loss.PB(loss);
    uint64_t iteration = 0;
    max_iterations = limit;

    while (fabs(loss - old_loss) > epsilon && iteration < max_iterations){
        // Calculate the gradients and update the weights and bias correctly. Do not edit anything else 
        if (iteration %100 == 0) train_loss.PB(loss);
        iteration++;

        auto result = lossDerivative(X,Y);
        double db=result.second;
        matrix dw=result.first;
        weights = weights + eta*dw;
        bias = bias + eta*db;

        old_loss = loss;
        loss = logisticLoss(X,Y);
    }
}

void LogisticRegression::train(matrix X,matrix Y,double learning_rate, uint64_t limit){
    GD(X,Y,learning_rate,limit);
    cout << "Training Loss\n";
    for (uint64_t i = 0; i < train_loss.size() ; i++){
        cout << train_loss[i] << "\n";
    }
}

void LogisticRegression::test(matrix X,matrix Y){
    matrix Y_pred(X.shape().first,1);
    Y_pred = predict(X);
    uint64_t n = X.shape().first;
    cout << "Predictions(GD) \t True Value\n"; 
    for (uint64_t i = 0 ; i < n ; i++){
        cout << Y_pred(i,0) <<  "\t\t\t " << Y(i,0) << "\n";
    }
    cout << "Testing loss " << logisticLoss(X,Y) << "\n";
    cout << "Testing accuracy " << accuracy(Y_pred,Y) << "\n";
}

double LogisticRegression::accuracy(matrix Y_pred, matrix Y){
    double acc = 0;
    // Compute the accuracy of the model
    uint64_t j = 0; 
    for(uint64_t i=0; i<Y.shape().first; ++i){
        if(Y_pred(i,0)==Y(i,0)){
            j++;
        }
    }
    acc = float(j)/Y.shape().first;
    return acc;
}

pair<pair<matrix, matrix>, pair<matrix, matrix>> test_train_split(matrix X, matrix Y, float ratio) {
    if (ratio < 0 || ratio > 1) {
        throw std::invalid_argument("Ratio must be between 0 and 1");
    }
    uint64_t n = X.shape().first;
    uint64_t train_size = static_cast<uint64_t>(n * ratio);
    uint64_t test_size = n - train_size;

    std::vector<uint64_t> indices(n);
    std::iota(indices.begin(), indices.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    matrix X_train(train_size, X.shape().second);
    matrix Y_train(train_size, 1);
    matrix X_test(test_size, X.shape().second);
    matrix Y_test(test_size, 1);

    for (uint64_t i = 0; i < train_size; ++i) {
        for (uint64_t j = 0; j < X.shape().second; ++j) {
            X_train(i, j) = X(indices[i], j);
        }
        Y_train(i, 0) = Y(indices[i], 0);
    }

    for (uint64_t i = 0; i < test_size; ++i) {
        for (uint64_t j = 0; j < X.shape().second; ++j) {
            X_test(i, j) = X(indices[train_size + i], j);
        }
        Y_test(i, 0) = Y(indices[train_size + i], 0);
    }

    return {{X_train, Y_train}, {X_test, Y_test}};
}