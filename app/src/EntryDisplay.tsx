import { useEffect, useState } from "react";
import { db } from "./firebase";
import {
  child,
  DataSnapshot,
  get,
  onValue,
  ref,
  remove,
} from "firebase/database";
import { FaTrash } from "react-icons/fa";
import toast from "react-hot-toast";

export interface EntryDisplayProps {
  title: string;
  path: string;
}

export function EntryDisplay({ path, title }: EntryDisplayProps) {
  const [entries, setEntries] = useState<string[] | null>(null);

  useEffect(() => {
    const valueHandler = (snapshot: DataSnapshot) => {
      const newEntries: string[] = [];
      snapshot.forEach((childSnapshot) => {
        newEntries.push(childSnapshot.val() as string);
      });
      setEntries(newEntries);
    };

    const entriesRef = ref(db, path);
    const unsubEntries = onValue(entriesRef, valueHandler);

    return () => {
      unsubEntries();
    };
  }, [path]);

  const deleteAllEntries = async () => {
    const entriesRef = ref(db, path);
    const snapshot = await get(entriesRef);
    snapshot.forEach((childSnapshot) => {
      remove(childSnapshot.ref);
    });
    toast.success("Deleted all entries");
  };

  if (entries === null) return <div>Loading...</div>;

  return (
    <div className="flex flex-col gap-1 h-96">
      <div className="flex flex-row justify-between items-baseline">
        <div></div>
        <h2 className="text-2xl text-center">{title}</h2>
        <button onClick={deleteAllEntries} className="btn btn-sm btn-outline">
          <FaTrash />
        </button>
      </div>

      <div className="w-full overflow-scroll">
        <div>
          {entries.map((entry, i) => (
            <div key={i} className="bg-gray-800 text-white p-4 rounded-lg mb-4">
              <p className="text-sm text-left">{entry}</p>
            </div>
          ))}
        </div>
      </div>
    </div>
  );
}
