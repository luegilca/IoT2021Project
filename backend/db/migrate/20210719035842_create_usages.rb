class CreateUsages < ActiveRecord::Migration[5.2]
  def change
    create_table :usages do |t|
      t.references :device, foreign_key: true

      t.timestamps
    end
  end
end
