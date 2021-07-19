class CreateDevices < ActiveRecord::Migration[5.2]
  def change
    create_table :devices do |t|
      t.integer :device_id
      t.float :max_capacity_ml
      t.float :dispensation_ml
      t.float :alert_capacity_ml
      t.boolean :status
      t.string :latitude
      t.string :longitude
      t.timestamps
    end
  end
end
