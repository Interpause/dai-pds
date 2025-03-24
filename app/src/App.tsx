import { EntryDisplay } from "./EntryDisplay";

function App() {
  return (
    <div className="p-8 max-w-prose mx-auto">
      <h1 className="text-3xl text-center">Doorbell Console</h1>
      <div className="flex flex-col gap-4">
        <EntryDisplay path="/debug" title="Debug Log" />
        <EntryDisplay path="/visitors" title="Visitor Log" />
      </div>
    </div>
  );
}

export default App;
