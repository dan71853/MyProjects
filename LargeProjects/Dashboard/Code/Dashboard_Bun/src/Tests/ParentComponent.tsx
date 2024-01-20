/**
 * THis is a example of useState made by chat gpt
 */



import React, { useState, useCallback } from 'react';

// CounterDisplay component
interface CounterDisplayProps {
  count: number;
}

const CounterDisplay: React.FC<CounterDisplayProps> = ({ count }) => {
  return <p>Count: {count}</p>;
};

// IncrementButton component
interface IncrementButtonProps {
  onIncrement: () => void;
}

const IncrementButton: React.FC<IncrementButtonProps> = ({ onIncrement }) => {
  return <button onClick={onIncrement}>Increment</button>;
};

// Parent component
const ParentComponent: React.FC = () => {
  const [count, setCount] = useState<number>(0);

  return (
    <div>
      <CounterDisplay count={count} />
      <IncrementButton
        onIncrement={useCallback(() => {
          setCount((prevCount) => prevCount + 1);
        }, [])}
      />
    </div>
  );
};

export default ParentComponent;
