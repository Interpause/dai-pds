import { AudioInput } from "./Audio";
import { EntryDisplay } from "./EntryDisplay";

function App() {
  return (
    <div className="p-8 max-w-prose mx-auto">
      <h1 className="text-3xl text-center mb-2">Doorbell Console</h1>
      <h3 className="text-2xl text-center mb-2">JSON Audio Input</h3>
      <AudioInput />
      <div className="flex flex-col gap-4 mt-4">
        <EntryDisplay path="/debug" title="Debug Log" />
        <EntryDisplay path="/visitors" title="Visitor Log" />
      </div>
    </div>
  );
}

export default App;
