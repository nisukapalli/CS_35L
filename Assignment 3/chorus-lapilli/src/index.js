import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';

function Square(props) {
    return(
        <button className="square" onClick={props.onClick}>
            {props.value}
        </button>
    );
}
  class Board extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            squares: Array(9).fill(null),
            xIsNext: true,
            nTurns: 0,
            lastSquare: -1,
        }
    }

    handleClick(i) {
      const squares = this.state.squares.slice();
      let player = this.state.xIsNext ? 'X' : 'O';

      if (calculateWinner(squares)) {
        return;
      }

      //play tic-tac-toe
      if (this.state.nTurns < 6) {
        if(squares[i]) {
          return;
        }
        squares[i] = player;
        this.setState({
          squares: squares,
          nTurns: this.state.nTurns + 1,
          xIsNext: !this.state.xIsNext
        });
      }
      
      //play chorus lapilli
      else {
        //if a square hasn't been selected
        if (this.state.lastSquare < 0) {
          //if the player occupies the square
          if(squares[i] === player) {
            squares[i] = null;
            this.setState({
              squares: squares,
              lastSquare: i,
            });
          }
          return;
        }
        //move piece to new square
        else {
          let center = (squares[4] === player);

          //if player clicks on the same square, put the mark back
          if (i === this.state.lastSquare) {
            squares[i] = player;
            this.setState({
              squares: squares,
              lastSquare: -1,
            });
            return;
          }
          
          //start over if player moves to a non-empty or non-adjacent square
          if (!isAdjacent(i, this.state.lastSquare) || squares[i]) {
            squares[this.state.lastSquare] = player;
            this.setState({
              squares: squares,
              lastSquare: -1,
            });
            return;
          }

          if (center) {
            squares[i] = player;
            if (!calculateWinner(squares)) {
              squares[i] = null;
              return;
            }
          }

          squares[i] = player;
          squares[this.state.lastSquare] = null;
          this.setState({
            squares: squares,
            lastSquare: -1,
            xIsNext: !this.state.xIsNext
          });
          return;
        }
      }
    }

    renderSquare(i) {
      return (
        <Square 
            value={this.state.squares[i]}
            onClick={() => this.handleClick(i)}
        />
      );
    }
    
  
    render() {
        const winner = calculateWinner(this.state.squares);
        let status;
        if (winner) {
          status = 'Winner: ' + winner;
        } else {
          status = 'Next player: ' + (this.state.xIsNext ? 'X' : 'O');
        }

      return (
        <div>
          <div className="status">{status}</div>
          <div className="board-row">
            {this.renderSquare(0)}
            {this.renderSquare(1)}
            {this.renderSquare(2)}
          </div>
          <div className="board-row">
            {this.renderSquare(3)}
            {this.renderSquare(4)}
            {this.renderSquare(5)}
          </div>
          <div className="board-row">
            {this.renderSquare(6)}
            {this.renderSquare(7)}
            {this.renderSquare(8)}
          </div>
        </div>
      );
    }
  }
  
  class Game extends React.Component {
    render() {
      return (
        <div className="game">
          <div className="game-board">
            <Board />
          </div>
          <div className="game-info">
            <div>{/* status */}</div>
            <ol>{/* TODOn't */}</ol>
          </div>
        </div>
      );
    }
  }
  
  // ========================================
  
  ReactDOM.render(
    <Game />,
    document.getElementById('root')
  );
  
  function calculateWinner(squares) {
    const lines = [
      [0, 1, 2],
      [3, 4, 5],
      [6, 7, 8],
      [0, 3, 6],
      [1, 4, 7],
      [2, 5, 8],
      [0, 4, 8],
      [2, 4, 6],
    ];
    for (let i = 0; i < lines.length; i++) {
      const [a, b, c] = lines[i];
      if (squares[a] && squares[a] === squares[b] && squares[a] === squares[c]) {
        return squares[a];
      }
    }
    return null;
  }

function isAdjacent(i, j) {
  if (i === 0)
    return j === 1 || j === 3 || j === 4;
  else if (i === 1)
    return j === 0 || j === 2 || j === 4 || j === 3 || j === 5;
  else if (i === 2)
    return j === 1 || j === 4 || j === 5;
  else if (i === 3)
    return j === 0 || j === 1 || j === 4 || j === 6 || j === 7;
  else if (i === 4) 
    return true;
  else if (i === 5)
    return j === 1 || j === 2 || j === 4 || j === 7 || j === 8;
  else if (i === 6)
    return j === 3 || j === 4 || j === 7;
  else if (i === 7)
    return j === 3 || j === 4 || j === 5 || j === 6 || j === 8;
  else if (i === 8)
    return j === 4 || j === 5 || j === 7;
}
