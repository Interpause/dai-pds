import { useState } from "react";
import toast from "react-hot-toast";
import { db } from "./firebase";
import { ref, set } from "firebase/database";

export function AudioInput() {
  const [input, setInput] = useState<string>("");

  const handleSubmit = () => {
    let list: number[] = [];
    try {
      list = JSON.parse(input);
      // Check if list is list of integers.
      if (!Array.isArray(list) || !list.every(Number.isInteger)) {
        toast.error("Invalid JSON");
        return;
      }
    } catch {
      toast.error("Invalid JSON");
      return;
    }
    const songRef = ref(db, "/song");
    set(songRef, list)
      .then(() => {
        toast.success("Audio tones updated");
      })
      .catch(() => {
        toast.error("Failed to update audio tones");
      });
  };

  return (
    <div className="w-full flex flex-row justify-center">
      <div className="join">
        <label className="input input-md join-item">
          <input
            placeholder="JSON List"
            required
            value={input}
            onChange={(e) => setInput(e.target.value)}
          />
        </label>
        <button
          className="btn btn-md btn-neutral join-item"
          onClick={handleSubmit}
        >
          Submit
        </button>
      </div>
    </div>
  );
}
