package com.cradtinginterpreters.lox;
import java.util.List;
abstract class Expr {
  interface Visitor<R> {
  R visitBinaryExpr(Binary expr);
  R visitGroupingExpr(Grouping expr);
  R visitLiteralExpr(Literal expr);
  R visitUnaryExpr(Unary expr);
  }
    Binary(Expr left, Token operator, Expr right) {
        this.left = left;
        this.operator = operator;
        this.right = right;
    }
   final Expr left;
   final Token operator;
   final Expr right;
  }

    @Override
    <R> R accept(Visitor<R> visitor) {
      return visitor.visitBinaryExpr(this);
    }    Grouping(Expr expression) {
        this.expression = expression;
    }
   final Expr expression;
  }

    @Override
    <R> R accept(Visitor<R> visitor) {
      return visitor.visitGroupingExpr(this);
    }    Literal(Object value) {
        this.value = value;
    }
   final Object value;
  }

    @Override
    <R> R accept(Visitor<R> visitor) {
      return visitor.visitLiteralExpr(this);
    }    Unary(Token operator, Expr right) {
        this.operator = operator;
        this.right = right;
    }
   final Token operator;
   final Expr right;
  }

    @Override
    <R> R accept(Visitor<R> visitor) {
      return visitor.visitUnaryExpr(this);
    }
    abstract <R> R accept(Visit<R> visitor);
}
